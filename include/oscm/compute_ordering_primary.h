#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H

#include <oscm/count_crossings.h>
#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/is_directed_acyclic.h>
#include <oscm/partial_order_fixed_points.h>
#include <oscm/transitive_reduction.h>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

namespace oscm {
    void compute_ordering_primary(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &edges_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_uppper_bound) {
        std::vector<std::vector<std::uint32_t>> directed_edges(nB_);
        find_pattern_and_set_edge(nA_, nB_, edges_, directed_edges);

        assert(is_directed_acyclic(directed_edges));

        auto modified_directed_edges = transitive_reduction(directed_edges);

        std::cout << "nA_ = " << nA_ << ", nB_ = " << nB_ << '\n';

        std::cout << "modified_directed_edges:\n";
        for(std::uint32_t i = 0; i < modified_directed_edges.size(); i++) {
            for(auto next : modified_directed_edges[i]) {
                std::cout << i << ' ' << next << '\n';
            }
        }
        std::cout << std::endl;

        ordering_ = topological_sort(directed_edges);
        for(auto &now: ordering_) { now += nA_; }
        if(auto const crossing = count_crossings(nA_, nB_, ordering_.data(), edges_);
           crossing < crossing_uppper_bound) {
            crossing_uppper_bound = crossing;
        }
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
