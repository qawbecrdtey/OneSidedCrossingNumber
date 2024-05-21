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
      std::vector<std::vector<std::uint32_t>> const &directed_edges_,
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

        compute_ordering_inner(
          nA_, nB_, connections_, directed_edges, ordering_, crossing_upper_bound_);
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

        if(auto const crossing_number =
             count_crossings(nA_, fixed_points.size(), fixed_points.data(), connections_);
           crossing_number > crossing_upper_bound_) {
            return;
        }
        else if(fixed_points.size() == nB_) {
            if(crossing_number < crossing_upper_bound_) {
                crossing_upper_bound_ = crossing_number;
                ordering_ = std::move(topological_ordering);
                for(auto &now: ordering_) { now += nA_; }
            }
            return;
        }

        auto next_directed_edges = transitive_reduction(directed_edges_);

        std::uint32_t l = 0;
        for(auto const fixed_vertex: fixed_points) {
            std::uint32_t j;
            for(j = l; topological_ordering[j] != fixed_vertex; j++) {
                for(std::uint32_t k = j + 1; topological_ordering[k] != fixed_vertex; k++) {
                    if(incomparable(
                         topological_ordering[j], topological_ordering[k], directed_edges_)) {
                        next_directed_edges[topological_ordering[j]].push_back(
                          topological_ordering[k]);
                        compute_ordering_inner(
                          nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);

                        next_directed_edges[topological_ordering[j]].pop_back();
                        next_directed_edges[topological_ordering[k]].push_back(
                          topological_ordering[j]);
                        compute_ordering_inner(
                          nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);
                        return;
                    }
                }
            }
            l = j + 1;
        }
        for(std::uint32_t j = l; j < topological_ordering.size(); j++) {
            for(std::uint32_t k = j + 1; k < topological_ordering.size(); k++) {
                if(incomparable(topological_ordering[j], topological_ordering[k], directed_edges_)) {
                    next_directed_edges[topological_ordering[j]].push_back(topological_ordering[k]);
                    compute_ordering_inner(
                      nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);

                    next_directed_edges[topological_ordering[j]].pop_back();
                    next_directed_edges[topological_ordering[k]].push_back(topological_ordering[j]);
                    compute_ordering_inner(
                      nA_, nB_, connections_, next_directed_edges, ordering_, crossing_upper_bound_);
                    return;
                }
            }
        }

        std::cerr << "If you see this message, then something has gone wrong." << std::endl;
        assert(false);
        __builtin_unreachable();
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
