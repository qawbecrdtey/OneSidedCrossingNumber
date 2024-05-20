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

    inline void compute_ordering(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> &ordering_,
      std::uint64_t &crossing_upper_bound_) {
        compute_ordering_inner(nA_, nB_, connections_, ordering_, crossing_upper_bound_);
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
            for(auto &now: ordering_) { now += nA_; }
            return;
        }

// TODO: FIX BELOW.

        std::cout << "directed_edges:\n";
        for(std::uint32_t i = 0; i < nB_; i++) {
            std::cout << ' ' << i << ":\t";
            for(auto next: directed_edges[i]) { std::cout << next << ' '; }
            std::cout << '\n';
        }
        std::cout << std::endl;

        std::cout << "fixed_points:\n";
        for(auto now: fixed_points) { std::cout << now << ' '; }
        std::cout << std::endl;

        std::cout << "topological_ordering:\n";
        for(auto now: topological_ordering) { std::cout << now << ' '; }
        std::cout << std::endl;

        std::cerr << "If you see this message, then something has gone wrong." << std::endl;
        assert(false);
        __builtin_unreachable();
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMPUTE_ORDERING_H
