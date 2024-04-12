#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H

#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/transitive_reduction.h>

#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

namespace oscm {
    void compute_ordering(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &edges_,
      std::vector<std::uint32_t> &ordering_,
      std::uint32_t const crossing_uppper_bound) {
        std::vector<std::vector<std::uint32_t>> directed_edges(nB_);
        find_pattern_and_set_edge(nA_, nB_, edges_, directed_edges);

        std::cout << "ordering:\n";
        for(std::uint32_t i = 0; i < nB_; i++) {
            for(auto const next : directed_edges[i]) {
                std::cout << i << ' ' << next << '\n';
            }
        }
        std::cout << std::endl;


        std::cout << "transitive reduced ordering:\n";
        directed_edges = transitive_reduction(directed_edges);
        for(std::uint32_t i = 0; i < nB_; i++) {
            for(auto const next : directed_edges[i]) {
                std::cout << i << ' ' << next << '\n';
            }
        }
        std::cout << std::endl;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
