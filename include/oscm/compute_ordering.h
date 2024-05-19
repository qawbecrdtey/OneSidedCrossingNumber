#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H

#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/is_directed_acyclic.h>
#include <oscm/partial_order_fixed_points.h>
#include <oscm/topological_sort.h>
#include <oscm/transitive_reduction.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    void compute_ordering_inner(
      std::uint32_t nA_,
      std::uint32_t nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_);

    void compute_ordering(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        compute_ordering_inner(nA_, nB_, connections_, ordering_, crossing_upper_bound_);
        for(auto &now: ordering_) { now += nA_; }
    }

    void compute_ordering_inner(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        std::vector<std::vector<std::uint32_t>> directed_edges(nB_);
        find_pattern_and_set_edge(nA_, nB_, connections_, directed_edges);

        auto [fixed_points, topological_ordering] =
          partial_order_fixed_points_with_topological_ordering(directed_edges);

        if(auto const crossing_number =
             count_crossings(nA_, fixed_points.size(), fixed_points.data(), connections_);
           crossing_number > crossing_upper_bound_) {
            return;
        }
        else if(fixed_points.size() == nB_ && crossing_number < crossing_upper_bound_) {
            crossing_upper_bound_ = crossing_number;
            ordering_ = std::move(topological_ordering);
            return;
        }

        // TODO: check for each segment of fixed_points.

        assert(is_directed_acyclic(directed_edges));
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
