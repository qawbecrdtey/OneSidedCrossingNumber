#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H

#include <oscm/C_storage.h>
#include <oscm/brute_force_ordering.h>
#include <oscm/comparable.h>
#include <oscm/exists_path.h>
#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/obtain_random_interval.h>
#include <oscm/partial_order_fixed_points.h>
#include <oscm/random_unsigned_integer.h>
#include <oscm/transitive_reduction.h>

#include <algorithm>
#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    void compute_ordering_inner(
      std::uint32_t nA_,
      std::uint32_t nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_);

    inline void compute_ordering(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        std::vector<std::vector<std::uint32_t>> directed_edges(nB_);
        find_pattern_and_set_edge(nA_, nB_, connections_, directed_edges);

        auto new_ordering = topological_sort(directed_edges);
        directed_edges = transitive_reduction(directed_edges, new_ordering);
        for(auto &now: new_ordering) { now += nA_; }
        if(auto const new_crossings = count_crossings(nA_, nB_, new_ordering.data(), connections_);
           new_crossings < crossing_upper_bound_) {
            crossing_upper_bound_ = new_crossings;
            ordering_ = std::move(new_ordering);
        }

        for(auto &now: ordering_) {
#if __has_cpp_attribute(assume)
            [[assume(now >= nA_)]];
#endif
            now -= nA_;
        }

        compute_ordering_inner(
          nA_, nB_, connections_, directed_edges, ordering_, crossing_upper_bound_);

        for(auto &now: ordering_) { now += nA_; }
    }

    inline void compute_ordering_inner(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        auto [fixed_points, topological_ordering, topological_ordering_inverse] =
          partial_order_fixed_points_with_topological_ordering(directed_edges_);

        for(auto &now: fixed_points) { now += nA_; }

        if(auto const crossing_number =
             count_crossings(nA_, fixed_points.size(), fixed_points.data(), connections_);
           crossing_number >= crossing_upper_bound_) {
            return;
        }
        else if(fixed_points.size() == nB_) {
            assert(crossing_number < crossing_upper_bound_);
#if __has_cpp_attribute(assume)
            [[assume(crossing_number < crossing_upper_bound_)]];
#endif
            crossing_upper_bound_ = crossing_number;
            ordering_ = std::move(topological_ordering);
            return;
        }

        for(auto &now: fixed_points) { now -= nA_; }

        std::vector<std::vector<std::uint32_t>> next_directed_edges;
        if(!random_unsigned_integer(0, nA_ + nB_)) {
            next_directed_edges = transitive_reduction(
              directed_edges_, topological_ordering, std::move(topological_ordering_inverse));
        }
        else { next_directed_edges = std::move(directed_edges_); }

        // auto next_directed_edges = transitive_reduction(
        //   directed_edges_, topological_ordering, std::move(topological_ordering_inverse));

        // auto const [left, right] = obtain_random_interval(fixed_points, topological_ordering);
        auto const [left, right] = obtain_random_interval_with_high_priority_on_longer_interval(
          fixed_points, topological_ordering);

        assert(left + 2 <= right);
#if __has_cpp_attribute(assume)
        [[assume(right - left >= 2)]];
#endif
        switch(right - left) {
        case 2: {
            assert(incomparable(
              topological_ordering[left], topological_ordering[left + 1], next_directed_edges));
            bool const b = C_get(topological_ordering[left], topological_ordering[left + 1]);
            next_directed_edges[topological_ordering[left + !b]].push_back(
              topological_ordering[left + b]);
            compute_ordering_inner(
              nA_, nB_, connections_, std::move(next_directed_edges), ordering_, crossing_upper_bound_);
        } break;
        case 3: {
            brute_force_ordering<3>(
              next_directed_edges, connections_, topological_ordering, nA_, left);
            compute_ordering_inner(
              nA_, nB_, connections_, std::move(next_directed_edges), ordering_, crossing_upper_bound_);
        } break;
        case 4: {
            brute_force_ordering<4>(
              next_directed_edges, connections_, topological_ordering, nA_, left);
            compute_ordering_inner(
              nA_, nB_, connections_, std::move(next_directed_edges), ordering_, crossing_upper_bound_);
        } break;
        case 5: {
            brute_force_ordering<5>(
              next_directed_edges, connections_, topological_ordering, nA_, left);
            compute_ordering_inner(
              nA_, nB_, connections_, std::move(next_directed_edges), ordering_, crossing_upper_bound_);
        } break;
        case 6: {
            brute_force_ordering<6>(
              next_directed_edges, connections_, topological_ordering, nA_, left);
            compute_ordering_inner(
              nA_, nB_, connections_, std::move(next_directed_edges), ordering_, crossing_upper_bound_);
        } break;
            // case 7: {
            //     brute_force_ordering<7>(
            //       next_directed_edges, connections_, topological_ordering, nA_, left);
            //     compute_ordering_inner(
            //       nA_, nB_, connections_, std::move(next_directed_edges), ordering_,
            //       crossing_upper_bound_);
            // } break;
            // case 8: {
            //     brute_force_ordering<8>(
            //       next_directed_edges, connections_, topological_ordering, nA_, left);
            //     compute_ordering_inner(
            //       nA_, nB_, connections_, std::move(next_directed_edges), ordering_,
            //       crossing_upper_bound_);
            // } break;

            // Cases up to brute_force_constant defined in the main cpp file.
            // Up to case 2k -> piv's range is [left + k, right - k - 1].
            // Up to case 2k + 1 -> piv's range is [left + k + 1, right - k - 1].
        default: {
            // 1.
            auto const piv = random_unsigned_integer(
              left + (brute_force_constant >> 1) + (brute_force_constant & 1),
              right - (brute_force_constant >> 1) - 1);

            // 2.
            // std::uint32_t piv;
            // switch(random_unsigned_integer(1, 3)) {
            // case 1: piv = (3 * left + right) >> 2; break;
            // case 2: piv = (left + right) >> 1; break;
            // case 3: piv = (left + 3 * right) >> 2; break;
            // default:
            // #if __cplusplus >= 202'302L
            //     std::unreachable();
            // #else
            //     __builtin_unreachable();
            // #endif
            // }

            // 3.
            // auto const piv = (left + right) >> 1;

            // 4.
            // auto const piv =
            //   random_unsigned_integer(0, 1) ? ((3 * left + right) >> 2) : ((left + right) >> 1);

            // 5.
            // auto const piv = left + 6;

            assert(left <= piv && piv < right);
            for(std::uint32_t i = left; i < right; i++) {
                if(incomparable(
                     topological_ordering[piv], topological_ordering[i], next_directed_edges)) {
                    auto const l = topological_ordering[piv];
                    auto const r = topological_ordering[i];

                    for(auto &now: topological_ordering) { now += nA_; }

                    if(auto const current_crossing =
                         count_crossings(nA_, nB_, topological_ordering.data(), connections_);
                       crossing_upper_bound_ > current_crossing) {
                        crossing_upper_bound_ = current_crossing;
                        for(auto &now: topological_ordering) { now -= nA_; }
                        ordering_ = topological_ordering;
                    }

                    if(C_get(l, r)) {
                        next_directed_edges[l].push_back(r);
                        compute_ordering_inner(
                          nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);

                        next_directed_edges[l].pop_back();
                        next_directed_edges[r].push_back(l);
                        compute_ordering_inner(
                          nA_,
                          nB_,
                          connections_,
                          std::move(next_directed_edges),
                          ordering_,
                          crossing_upper_bound_);
                    }
                    else {
                        next_directed_edges[r].push_back(l);
                        compute_ordering_inner(
                          nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);

                        next_directed_edges[r].pop_back();
                        next_directed_edges[l].push_back(r);
                        compute_ordering_inner(
                          nA_,
                          nB_,
                          connections_,
                          std::move(next_directed_edges),
                          ordering_,
                          crossing_upper_bound_);
                    }
                    return;
                }
            }
        } break;
        }
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
