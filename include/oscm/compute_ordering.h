#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H

#include <oscm/C_storage.h>
#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/obtain_random_interval.h>
#include <oscm/partial_order_fixed_points.h>
#include <oscm/random_unsigned_integer.h>
#include <oscm/transitive_reduction.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    void compute_ordering_inner(
      std::uint32_t nA_,
      std::uint32_t nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_);

    __attribute__((always_inline)) inline void compute_ordering(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        std::vector<std::vector<std::uint32_t>> directed_edges(nB_);
        find_pattern_and_set_edge(nA_, nB_, connections_, directed_edges);

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
      std::vector<std::vector<std::uint32_t>> const &directed_edges_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        auto [fixed_points, topological_ordering] =
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

        auto next_directed_edges = transitive_reduction(std::move(directed_edges_));

        auto const [left, right] = obtain_random_interval(fixed_points, topological_ordering);

        assert(left + 2 <= right);

        if(right - left == 2) {  // Apply RRLO2 reduction rule.
            assert(incomparable(
              topological_ordering[left], topological_ordering[left + 1], next_directed_edges));
            bool const b = C_get(topological_ordering[left], topological_ordering[left + 1]);
            next_directed_edges[topological_ordering[left + (!b)]].push_back(
              topological_ordering[left + b]);
            compute_ordering_inner(
              nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);
            return;
        }

        auto const piv = random_unsigned_integer(left, right - 1);
        assert(left <= piv && piv < right);
        for(std::uint32_t i = left; i < right; i++) {
            if(incomparable(topological_ordering[piv], topological_ordering[i], next_directed_edges)) {
                next_directed_edges[topological_ordering[piv]].push_back(topological_ordering[i]);
                compute_ordering_inner(
                  nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);

                next_directed_edges[topological_ordering[piv]].pop_back();
                next_directed_edges[topological_ordering[i]].push_back(topological_ordering[piv]);
                compute_ordering_inner(
                  nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);
                return;
            }
        }

        assert(false);
#if __cplusplus >= 202'302L
        std::unreachable();
#else
        __builtin_unreachable();
#endif
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
