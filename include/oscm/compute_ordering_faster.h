#ifndef OSCM_COMPUTE_ORDERING_FASTER_H
#define OSCM_COMPUTE_ORDERING_FASTER_H

#include <oscm/bipartite_graph.h>
#include <oscm/brute_force_ordering.h>
#include <oscm/comparable.h>
#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/obtain_all_intervals.h>
#include <oscm/partial_order_fixed_points.h>
#include <oscm/random_unsigned_integer.h>
#include <oscm/release_vector_memory.h>
#include <oscm/topological_sort.h>
#include <oscm/transitive_reduction.h>

#include <cstdint>
#include <numeric>
#include <utility>
#include <vector>

namespace oscm {

    void compute_ordering_faster_inner(
      std::uint32_t nA_,
      std::uint32_t nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      std::vector<std::uint32_t> directed_edges_index_to_ordering_index_map_,
      std::vector<std::uint32_t> directed_edges_index_to_ordering_index_map_inverse_,
      std::vector<std::uint32_t> &ordering_,
      std::vector<std::uint32_t>::iterator ordering_begin_,
      std::vector<std::uint32_t>::iterator ordering_end_,
      std::uint64_t &crossing_upper_bound_);

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

        std::vector<std::uint32_t> directed_edges_index_to_ordering_index_map(nB_);
        std::iota(
          directed_edges_index_to_ordering_index_map.begin(),
          directed_edges_index_to_ordering_index_map.end(),
          0);
        std::vector<std::uint32_t> directed_edges_index_to_ordering_index_map_inverse(nB_);
        std::copy_n(
          directed_edges_index_to_ordering_index_map.begin(),
          nB_,
          directed_edges_index_to_ordering_index_map_inverse.begin());

        compute_ordering_faster_inner(
          nA_,
          nB_,
          connections_,
          std::move(directed_edges),
          std::move(directed_edges_index_to_ordering_index_map),
          std::move(directed_edges_index_to_ordering_index_map_inverse),
          ordering_,
          ordering_.begin(),
          ordering_.end(),
          crossing_upper_bound_);

        for(auto &now: ordering_) { now += nA_; }
    }

    /**
     * The main algorithm.
     * @param nA_ Number of vertices of the fixed side of the bipartite graph, namely A.
     * @param nB_ Number of vertices of the other side of the bipartite graph, namely B.
     * @param connections_ A connection between A and B.
     * @param directed_edges_ An ordering of B which is determined at the beginning or by branching.
     * Each vertices are labeled from 0 to \p ordering_end_ - \p ordering_begin_ .
     * @param directed_edges_index_to_ordering_index_map_ A map from \p directed_edges_ to \p
     * ordering_ . Its length equals \p ordering_end_ - \p ordering_begin_ .
     * @param directed_edges_index_to_ordering_index_map_inverse_ An inverse map of \p
     * directed_edges_index_to_ordering_index_map_ . Its length is \p nB_ .
     * @param ordering_ The currently known best ordering obeying \p directed_edges_ .
     * @param ordering_begin_ The beginning of our interest.
     * @param ordering_end_ The end of our interest.
     * @param crossing_upper_bound_ The currently known best crossing number,
     * which is also the partial crossing number of \p ordering_ , from \p ordering_begin_ to \p
     * ordering_end_ .
     */
    inline void compute_ordering_faster_inner(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      std::vector<std::uint32_t> directed_edges_index_to_ordering_index_map_,
      std::vector<std::uint32_t> directed_edges_index_to_ordering_index_map_inverse_,
      std::vector<std::uint32_t> &ordering_,
      std::vector<std::uint32_t>::iterator ordering_begin_,
      std::vector<std::uint32_t>::iterator ordering_end_,
      std::uint64_t &crossing_upper_bound_) {
        assert(ordering_begin_ <= ordering_begin_);
        assert(ordering_begin_ <= ordering_end_);
        assert(ordering_end_ <= ordering_.end());
        assert(ordering_.size() == nB_);
        assert(
          directed_edges_.size()
          == static_cast<decltype(directed_edges_.size())>(ordering_end_ - ordering_begin_));
        assert(
          directed_edges_index_to_ordering_index_map_.size()
          == static_cast<decltype(directed_edges_index_to_ordering_index_map_.size())>(
            ordering_end_ - ordering_begin_));
        assert(
          directed_edges_index_to_ordering_index_map_inverse_.size()
          == static_cast<decltype(directed_edges_index_to_ordering_index_map_inverse_.size())>(nB_));
        [[assume(ordering_begin_ <= ordering_begin_)]];
        [[assume(ordering_begin_ <= ordering_end_)]];
        [[assume(ordering_end_ <= ordering_.end())]];
        [[assume(ordering_.size() == nB_)]];
        [[assume(
          directed_edges_.size()
          == static_cast<decltype(directed_edges_.size())>(ordering_end_ - ordering_begin_))]];
        [[assume(
          directed_edges_index_to_ordering_index_map_.size()
          == static_cast<decltype(directed_edges_index_to_ordering_index_map_.size())>(
            ordering_end_ - ordering_begin_))]];
        [[assume(
          directed_edges_index_to_ordering_index_map_inverse_.size()
          == static_cast<decltype(directed_edges_index_to_ordering_index_map_inverse_.size())>(nB_))]];

        auto [fixed_points, topological_ordering, topological_ordering_inverse] =
          partial_order_fixed_points_with_topological_ordering(directed_edges_);

        for(auto &now: fixed_points) {
            assert(now < directed_edges_index_to_ordering_index_map_.size());
            [[assume(now < directed_edges_index_to_ordering_index_map_.size())]];
            now = directed_edges_index_to_ordering_index_map_[now] + nA_;
            assert(nA_ <= now);
            assert(now < nA_ + nB_);
            [[assume(nA_ <= now)]];
            [[assume(now < nA_ + nB_)]];
        }

        if(auto const crossing_number = count_crossings(
             nA_, static_cast<std::uint32_t>(fixed_points.size()), fixed_points.data(), connections_);
           crossing_upper_bound_ <= crossing_number) {
            return;
        }
        else if(
          fixed_points.size()
          == static_cast<decltype(fixed_points.size())>(ordering_end_ - ordering_begin_)) {
            assert(fixed_points == topological_ordering);
            [[assume(fixed_points == topological_ordering)]];
            [[assume(crossing_number < crossing_upper_bound_)]];
            crossing_upper_bound_ = crossing_number;
            auto it = ordering_begin_;
            for(auto now: fixed_points) {
                *it = now;
                ++it;
            }
            assert(it == ordering_end_);
            [[assume(it == ordering_end_)]];
        }

        for(auto &now: fixed_points) {
            now = directed_edges_index_to_ordering_index_map_inverse_[now - nA_];
            assert(now < directed_edges_.size());
            [[assume(now < directed_edges_.size())]];
        }

        std::vector<std::vector<std::uint32_t>> new_directed_edges;
        if(!random_unsigned_integer(0, nA_ + directed_edges_.size())) {
            new_directed_edges = transitive_reduction(
              directed_edges_, topological_ordering, std::move(topological_ordering_inverse));
            release_vector_memory(directed_edges_);
        }
        else {
            new_directed_edges = std::move(directed_edges_);
            topological_ordering_inverse.reset();  // Release memory.
        }

        auto const intervals = obtain_all_intervals(fixed_points, topological_ordering);
        release_vector_memory(fixed_points);

        for(auto const [left, right]: intervals) {
            assert(left + 2 <= right);
            [[assume(left + 2 <= right)]];
            switch(right - left) {
            case 2: {
                assert(incomparable(
                  topological_ordering[left], topological_ordering[left + 1], new_directed_edges));
                [[assume(incomparable(
                  topological_ordering[left], topological_ordering[left + 1], new_directed_edges))]];
                brute_force_ordering<2>(
                  new_directed_edges, connections_, topological_ordering, nA_, left);
            } break;
            case 3: {
                brute_force_ordering<3>(
                  new_directed_edges, connections_, topological_ordering, nA_, left);
            } break;
            case 4: {
                brute_force_ordering<4>(
                  new_directed_edges, connections_, topological_ordering, nA_, left);
            } break;
            case 5: {
                brute_force_ordering<5>(
                  new_directed_edges, connections_, topological_ordering, nA_, left);
            } break;
            case 6: {
                brute_force_ordering<6>(
                  new_directed_edges, connections_, topological_ordering, nA_, left);
            } break;
            default: {

                /**
                 * TODO:
                 * next_directed_edges
                 * next_directed_edges_index_to_ordering_index_map
                 * next_directed_edges_index_to_ordering_index_map_inverse
                 * next_ordering_begin
                 * next_ordering_end
                 * next_crossing_upper_bound
                 */

                // compute_ordering_faster_inner(nA_, nB_, connections_, next_directed_edges, next_directed_edges_index_to_ordering_index_map, next_directed_edges_index_to_ordering_index_map_inverse, ordering_, next_ordering_begin, next_ordering_end, next_crossing_upper_bound);



#if __cplusplus >= 202'302L
                std::unreachable();
#else
                __builtin_unreachable();
#endif
            }
            }
        }
    }
}  // namespace oscm
#endif  // OSCM_COMPUTE_ORDERING_FASTER_H
