#ifndef OSCM_COMPUTE_ORDERING_FASTER_H
#define OSCM_COMPUTE_ORDERING_FASTER_H

#include <oscm/bipartite_graph.h>
#include <oscm/brute_force_ordering.h>
#include <oscm/comparable.h>
#include <oscm/contains_distinct_elements.h>
#include <oscm/contains_equal_elements.h>
#include <oscm/elements_in_range.h>
#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/obtain_all_intervals.h>
#include <oscm/partial_order_fixed_points.h>
#include <oscm/random_unsigned_integer.h>
#include <oscm/release_vector_memory.h>
#include <oscm/topological_sort.h>
#include <oscm/transitive_reduction.h>

#include <cstdint>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

namespace oscm {

    std::pair<std::vector<std::uint32_t>, std::uint64_t> compute_ordering_faster_inner(
      std::uint32_t nA_,
      std::uint32_t nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_inverse_,
      std::vector<std::uint32_t> const &ordering_,
      std::uint64_t crossing_upper_bound_);

    inline void compute_ordering_faster(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        std::vector<std::vector<std::uint32_t>> directed_edges(nB_);
        find_pattern_and_set_edge(nA_, nB_, connections_, directed_edges);

        auto new_ordering =
          topological_sort_degree_prioritized(nA_, nB_, directed_edges, connections_);
        directed_edges = transitive_reduction(directed_edges, new_ordering);

#if DEBUG_MESSAGE
        std::cout << "One topological ordering after finding patterns:\n";
        for(auto const now: new_ordering) { std::cout << now + nA_ << ' '; }
        std::cout << std::endl;
#endif

        for(auto &now: new_ordering) { now += nA_; }
        if(auto const new_crossings = count_crossings(nA_, nB_, new_ordering.data(), connections_);
           new_crossings < crossing_upper_bound_) {
            crossing_upper_bound_ = new_crossings;
            ordering_ = std::move(new_ordering);
        }
        else { release_vector_memory(new_ordering); }

#if DEBUG_MESSAGE
        std::cout << "Resulting initial ordering:\n";
        for(auto const now: ordering_) { std::cout << now << ' '; }
        std::cout << "\nResulting initial crossing_upper_bound: " << crossing_upper_bound_ << '\n';
#endif

        for(auto &now: ordering_) {
            assert(now >= nA_);
            [[assume(now >= nA_)]];
            now -= nA_;
            assert(now < nB_);
            [[assume(now < nB_)]];
        }

        std::vector<std::uint32_t> iota_map(nB_);
        std::iota(iota_map.begin(), iota_map.end(), 0);

        assert(ordering_.size() < std::numeric_limits<std::uint32_t>::max());
        [[assume(ordering_.size() < std::numeric_limits<std::uint32_t>::max())]];

        std::tie(ordering_, crossing_upper_bound_) = compute_ordering_faster_inner(
          nA_,
          nB_,
          connections_,
          std::move(directed_edges),
          iota_map,
          iota_map,
          ordering_,
          crossing_upper_bound_);

#if DEBUG_MESSAGE
        std::cout << "resulting crossing_upper_bound_ = " << crossing_upper_bound_ << '\n';
#endif

        for(auto &now: ordering_) { now += nA_; }
    }

    /**
     * The main algorithm.
     * @param nA_ Number of vertices of the fixed side of the bipartite graph, namely A.
     * @param nB_ Number of vertices of the other side of the bipartite graph, namely B.
     * @param connections_ A connection between A and B.
     * @param directed_edges_ An ordering of B which is determined at the beginning or by branching.
     * Each vertices are labeled from 0 to \p ordering_end_ - \p ordering_begin_ .
     * i -> j of \p directed_edges_ implies
     * \p directed_edges_index_to_B_map_ [\p i ] -> \p directed_edges_index_to_B_map_ [\p j ].
     * @param directed_edges_index_to_B_map_ A map from index of \p directed_edges_ to B .
     * It maps from [0, \p directed_edges_ .size()) to [0, \p nB_ ). Its length equals \p
     * ordering_end_idx_ - \p ordering_begin_idx_ = \p directed_edges_ .size().
     * @param directed_edges_index_to_B_map_inverse_ An inverse map of \p
     * directed_edges_index_to_B_map_ . Its length is \p nB_ .
     * @param ordering_ The currently known best ordering obeying \p directed_edges_ .
     * @param ordering_begin_idx_ The beginning index of our interest.
     * @param ordering_end_idx_ The end index of our interest.
     * @param crossing_upper_bound_ The currently known best crossing number,
     * which is also the partial crossing number of \p ordering_ , from \p ordering_begin_ to \p
     * ordering_end_ .
     */
    inline std::pair<std::vector<std::uint32_t>, std::uint64_t> compute_ordering_faster_inner(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_inverse_,
      std::vector<std::uint32_t> const &ordering_,
      std::uint64_t const crossing_upper_bound_) {
        assert(elements_in_range(ordering_, 0, ordering_.size()));
        assert(contains_distinct_elements(ordering_));

        auto [fixed_points, topological_ordering, topological_ordering_inverse] =
          partial_order_fixed_points_with_topological_ordering(directed_edges_);

        assert(topological_ordering.size() == directed_edges_.size());
        assert(elements_in_range(fixed_points, 0, directed_edges_.size()));
        assert(elements_in_range(topological_ordering, 0, directed_edges_.size()));
        assert(contains_distinct_elements(topological_ordering));
        [[assume(topological_ordering.size() == directed_edges_.size())]];
        [[assume(elements_in_range(fixed_points, 0, directed_edges_.size()))]];
        [[assume(elements_in_range(topological_ordering, 0, directed_edges_.size()))]];
        [[assume(contains_distinct_elements(topological_ordering))]];

#ifndef NDEBUG
        auto fixed_points_copy(fixed_points);
#endif

        for(auto &now: fixed_points) {
            assert(now < directed_edges_index_to_B_map_.size());
            [[assume(now < directed_edges_index_to_B_map_.size())]];
            now = directed_edges_index_to_B_map_[now] + nA_;
        }

        [[assume(fixed_points.size() < std::numeric_limits<std::uint32_t>::max())]];
        if(auto const crossing_number = count_crossings(
             nA_, static_cast<std::uint32_t>(fixed_points.size()), fixed_points.data(), connections_);
           crossing_upper_bound_ <= crossing_number) {
            return {ordering_, crossing_upper_bound_};
        }
        else if(fixed_points.size() == directed_edges_.size()) {
            [[assume(crossing_number < crossing_upper_bound_)]];
            return {std::move(topological_ordering), crossing_number};
        }

        for(auto &now: fixed_points) {
            assert(nA_ <= now && now < nA_ + nB_);
            assert(now - nA_ < directed_edges_index_to_B_map_inverse_.size());
            assert(
              directed_edges_index_to_B_map_inverse_[now - nA_]
              != std::numeric_limits<std::uint32_t>::max());
            [[assume(nA_ <= now && now < nA_ + nB_)]];
            [[assume(now - nA_ < directed_edges_index_to_B_map_inverse_.size())]];
            [[assume(
              directed_edges_index_to_B_map_inverse_[now - nA_]
              != std::numeric_limits<std::uint32_t>::max())]];
            now = directed_edges_index_to_B_map_inverse_[now - nA_];
            assert(now < directed_edges_index_to_B_map_.size());
            [[assume(now < directed_edges_index_to_B_map_.size())]];
        }

#ifndef NDEBUG
        assert(fixed_points == fixed_points_copy);
        [[assume(fixed_points == fixed_points_copy)]];
        release_vector_memory(fixed_points_copy);
#endif

        // TODO: Check if freeing some memory is possible.
        if(!random_unsigned_integer(0, nA_ + directed_edges_.size())) {
            directed_edges_ = transitive_reduction(
              directed_edges_, topological_ordering, topological_ordering_inverse.get());
        }

        // TODO: Check if freeing some memory is possible.
        auto const intervals = obtain_all_intervals(fixed_points, topological_ordering);
        release_vector_memory(fixed_points);
        assert(!intervals.empty());
        [[assume(!intervals.empty())]];

        for(auto const &[top_left, top_right]: intervals) {
            assert(top_left + 2 <= top_right);
            [[assume(top_left + 2 <= top_right)]];
            switch(top_right - top_left) {
            case 2:
                brute_force_ordering_modify_topological_ordering<2>(
                  directed_edges_,
                  connections_,
                  directed_edges_index_to_B_map_,
                  directed_edges_index_to_B_map_inverse_,
                  topological_ordering,
                  nA_,
                  top_left);
                break;
            case 3:
                brute_force_ordering_modify_topological_ordering<3>(
                  directed_edges_,
                  connections_,
                  directed_edges_index_to_B_map_,
                  directed_edges_index_to_B_map_inverse_,
                  topological_ordering,
                  nA_,
                  top_left);
                break;
            case 4:
                brute_force_ordering_modify_topological_ordering<4>(
                  directed_edges_,
                  connections_,
                  directed_edges_index_to_B_map_,
                  directed_edges_index_to_B_map_inverse_,
                  topological_ordering,
                  nA_,
                  top_left);
                break;
            case 5:
                brute_force_ordering_modify_topological_ordering<5>(
                  directed_edges_,
                  connections_,
                  directed_edges_index_to_B_map_,
                  directed_edges_index_to_B_map_inverse_,
                  topological_ordering,
                  nA_,
                  top_left);
                break;
            case 6:
                brute_force_ordering_modify_topological_ordering<6>(
                  directed_edges_,
                  connections_,
                  directed_edges_index_to_B_map_,
                  directed_edges_index_to_B_map_inverse_,
                  topological_ordering,
                  nA_,
                  top_left);
                break;
            default: {
                [[assume(top_left + 7 <= top_right)]];
                // auto const top_piv = random_unsigned_integer(top_left + 3, top_right - 4);
                auto const top_piv = (top_left + top_right) >> 1;
                assert(top_left <= top_piv && top_piv < top_right);
                [[assume(top_left <= top_piv && top_piv < top_right)]];

                std::vector<std::vector<std::uint32_t>> next_directed_edges(top_right - top_left);
                std::vector<std::uint32_t> next_ordering(top_right - top_left);
                std::iota(next_ordering.begin(), next_ordering.end(), 0);
                std::vector<std::uint32_t> next_directed_edges_index_to_B_map(top_right - top_left);
                std::vector<std::uint32_t> next_directed_edges_index_to_B_map_inverse(
                  nB_, std::numeric_limits<std::uint32_t>::max());

                for(std::uint32_t top_idx = top_left; top_idx < top_right; top_idx++) {
                    auto const dir_idx = topological_ordering[top_idx];
                    for(auto const next_dir_idx: directed_edges_[dir_idx]) {
                        assert(next_dir_idx < topological_ordering.size());
                        [[assume(next_dir_idx < topological_ordering.size())]];
                        auto const next_top_idx = topological_ordering_inverse[next_dir_idx];
                        if(next_top_idx < top_left || top_right <= next_top_idx) { continue; }
                        assert(top_left <= top_idx && top_idx < top_right);
                        assert(top_left <= next_top_idx && next_top_idx < top_right);
                        assert(top_idx - top_left < next_directed_edges.size());
                        assert(next_top_idx - top_left < next_directed_edges.size());
                        [[assume(top_left <= top_idx && top_idx < top_right)]];
                        [[assume(top_left <= next_top_idx && next_top_idx < top_right)]];
                        [[assume(top_idx - top_left < next_directed_edges.size())]];
                        [[assume(next_top_idx - top_left < next_directed_edges.size())]];
                        next_directed_edges[top_idx - top_left].push_back(next_top_idx - top_left);
                    }
                    assert(top_idx - top_left < next_directed_edges_index_to_B_map.size());
                    [[assume(top_idx - top_left < next_directed_edges_index_to_B_map.size())]];
                    [[assume(dir_idx < directed_edges_index_to_B_map_.size())]];
                    next_directed_edges_index_to_B_map[top_idx - top_left] =
                      directed_edges_index_to_B_map_[dir_idx];
                    assert(
                      next_directed_edges_index_to_B_map[top_idx - top_left]
                      < next_directed_edges_index_to_B_map_inverse.size());
                    [[assume(
                      next_directed_edges_index_to_B_map[top_idx - top_left]
                      < next_directed_edges_index_to_B_map_inverse.size())]];
                    next_directed_edges_index_to_B_map_inverse
                      [next_directed_edges_index_to_B_map[top_idx - top_left]] = top_idx - top_left;
                }

#if DEBUG_MESSAGE
# ifndef NDEBUG

                std::cout << "\nordering_: len = " << ordering_.size() << '\n';
                for(std::uint32_t i = 0; i < ordering_.size(); i++) {
                    std::cout << ordering_[i] << ' ';
                }
                std::cout << "\ndirected_edges_index_to_B_map_:\n";
                for(std::uint32_t i = 0; i < directed_edges_index_to_B_map_.size(); i++) {
                    std::cout << directed_edges_index_to_B_map_[i] << ' ';
                }
                std::cout << "\ndirected_edges_index_to_B_map_inverse_:\n";
                for(std::uint32_t i = 0; i < directed_edges_index_to_B_map_inverse_.size(); i++) {
                    std::cout << directed_edges_index_to_B_map_inverse_[i] << ' ';
                }
                std::cout << "\nnext_ordering: len = " << next_ordering.size() << '\n';
                for(std::uint32_t i = 0; i < top_right - top_left; i++) {
                    std::cout << next_ordering[i] << ' ';
                }
                std::cout << "\nnext_directed_edges_index_to_B_map:\n";
                for(std::uint32_t i = 0; i < top_right - top_left; i++) {
                    std::cout << next_directed_edges_index_to_B_map[i] << ' ';
                }
                std::cout << "\nnext_directed_edges_index_to_B_map_inverse:\n";
                assert(next_directed_edges_index_to_B_map_inverse.size() == nB_);
                for(std::uint32_t i = 0; i < nB_; i++) {
                    std::cout << next_directed_edges_index_to_B_map_inverse[i] << ' ';
                }
                std::cout << std::endl;
# endif
#endif

                std::uint64_t current_crossing_upper_bound;
                {
                    std::vector<std::uint32_t> subset_B;
                    subset_B.reserve(top_right - top_left);
                    for(std::uint32_t i = 0; i < top_right - top_left; i++) {
                        subset_B.push_back(next_directed_edges_index_to_B_map[i] + nA_);
                    }
                    current_crossing_upper_bound =
                      count_crossings(nA_, top_right - top_left, subset_B.data(), connections_);
                }

                for(std::uint32_t top_idx = top_left; top_idx < top_piv; top_idx++) {
                    auto const next_l = top_idx - top_left;
                    auto const next_r = top_piv - top_left;
                    assert(next_l < next_directed_edges.size());
                    assert(next_r < next_directed_edges.size());
                    if(exists_path(next_l, next_r, next_directed_edges)) { continue; }

                    next_directed_edges[next_l].push_back(next_r);
                    assert(is_directed_acyclic(next_directed_edges));
                    std::tie(next_ordering, current_crossing_upper_bound) = compute_ordering_faster_inner(
                      nA_,
                      nB_,
                      connections_,
                      next_directed_edges,
                      next_directed_edges_index_to_B_map,
                      next_directed_edges_index_to_B_map_inverse,
                      next_ordering,
                      current_crossing_upper_bound);

                    assert(elements_in_range(next_ordering, 0, next_ordering.size()));
                    assert(contains_distinct_elements(next_ordering));

                    next_directed_edges[next_l].pop_back();
                    next_directed_edges[next_r].push_back(next_l);
                    assert(is_directed_acyclic(next_directed_edges));
                    std::tie(next_ordering, current_crossing_upper_bound) = compute_ordering_faster_inner(
                      nA_,
                      nB_,
                      connections_,
                      std::move(next_directed_edges),
                      next_directed_edges_index_to_B_map,
                      next_directed_edges_index_to_B_map_inverse,
                      next_ordering,
                      current_crossing_upper_bound);

                    assert(elements_in_range(next_ordering, 0, next_ordering.size()));
                    assert(contains_distinct_elements(next_ordering));

                    for(std::uint32_t i = top_left; i < top_right; i++) {
                        topological_ordering[i] = directed_edges_index_to_B_map_inverse_
                          [next_directed_edges_index_to_B_map[next_ordering[i - top_left]]];
                    }
                    assert(contains_distinct_elements(topological_ordering));

                    goto END;
                }

                for(std::uint32_t top_idx = top_piv + 1; top_idx < top_right; top_idx++) {
                    auto const next_l = top_piv - top_left;
                    auto const next_r = top_idx - top_left;
                    assert(next_l < next_directed_edges.size());
                    assert(next_r < next_directed_edges.size());
                    if(exists_path(next_l, next_r, next_directed_edges)) { continue; }

                    next_directed_edges[next_l].push_back(next_r);
                    assert(is_directed_acyclic(next_directed_edges));
                    std::tie(next_ordering, current_crossing_upper_bound) = compute_ordering_faster_inner(
                      nA_,
                      nB_,
                      connections_,
                      next_directed_edges,
                      next_directed_edges_index_to_B_map,
                      next_directed_edges_index_to_B_map_inverse,
                      next_ordering,
                      current_crossing_upper_bound);

                    assert(elements_in_range(next_ordering, 0, next_ordering.size()));
                    assert(contains_distinct_elements(next_ordering));

                    [[assume(!next_directed_edges[next_l].empty())]];
                    next_directed_edges[next_l].pop_back();
                    next_directed_edges[next_r].push_back(next_l);
                    assert(is_directed_acyclic(next_directed_edges));
                    std::tie(next_ordering, current_crossing_upper_bound) = compute_ordering_faster_inner(
                      nA_,
                      nB_,
                      connections_,
                      std::move(next_directed_edges),
                      next_directed_edges_index_to_B_map,
                      next_directed_edges_index_to_B_map_inverse,
                      next_ordering,
                      current_crossing_upper_bound);

                    assert(elements_in_range(next_ordering, 0, next_ordering.size()));
                    assert(contains_distinct_elements(next_ordering));

                    for(std::uint32_t i = top_left; i < top_right; i++) {
                        topological_ordering[i] = directed_edges_index_to_B_map_inverse_
                          [next_directed_edges_index_to_B_map[next_ordering[i - top_left]]];
                    }
                    assert(contains_distinct_elements(topological_ordering));

                    goto END;
                }

#if __cplusplus >= 202'302L
                std::unreachable();
#else
                __builtin_unreachable();
#endif
            }
END:
                break;
            }
        }
        topological_ordering_inverse.reset();

        std::vector<std::uint32_t> subset_B;
        subset_B.reserve(ordering_.size());
        assert(topological_ordering.size() == ordering_.size());
        assert(contains_distinct_elements(topological_ordering));
        assert(elements_in_range(topological_ordering, 0, topological_ordering.size()));
        for(auto const top_idx: topological_ordering) {
            assert(top_idx < directed_edges_index_to_B_map_.size());
            subset_B.push_back(directed_edges_index_to_B_map_[top_idx] + nA_);
        }

        if(auto const current_crossing = count_crossings(
             nA_, static_cast<std::uint32_t>(subset_B.size()), subset_B.data(), connections_);
           crossing_upper_bound_ <= current_crossing) {
            return {ordering_, crossing_upper_bound_};
        }
        else { return {std::move(topological_ordering), current_crossing}; }
    }
}  // namespace oscm
#endif  // OSCM_COMPUTE_ORDERING_FASTER_H
