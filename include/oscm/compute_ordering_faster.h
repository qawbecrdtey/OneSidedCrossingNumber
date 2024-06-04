#ifndef OSCM_COMPUTE_ORDERING_FASTER_H
#define OSCM_COMPUTE_ORDERING_FASTER_H

#include "partial_order_fixed_points.h"
#include "topological_sort.h"
#include "transitive_reduction.h"

#include <oscm/bipartite_graph.h>
#include <oscm/find_pattern_and_set_edge.h>
#include <oscm/release_vector_memory.h>

#include <cstdint>
#include <numeric>
#include <utility>
#include <vector>

namespace oscm {

    void compute_ordering_faster_inner(
      std::uint32_t nA_,
      std::uint32_t nB_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t>::iterator ordering_start_,
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
#if __has_cpp_attribute(assume)
            [[assume(now >= nA_)]];
#endif
            now -= nA_;
        }

        std::vector<std::uint32_t> side_B(nB_);
        std::iota(side_B.begin(), side_B.end(), 1);

        bipartite_graph graph(nA_, std::move(side_B), connections_);

        compute_ordering_faster_inner(
          nA_,
          nB_,
          std::move(directed_edges),
          connections_,
          ordering_.begin(),
          ordering_.end(),
          crossing_upper_bound_);

        for(auto &now: ordering_) { now += nA_; }
    }

    inline void compute_ordering_faster_inner(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::vector<std::uint32_t>> directed_edges_,
      bipartite_graph graph_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t>::iterator ordering_start_,
      std::vector<std::uint32_t>::iterator ordering_end_,
      std::uint64_t &crossing_upper_bound_) {
        auto [fixed_points, topological_ordering, topological_ordering_inverse] =
          partial_order_fixed_points_with_topological_ordering(graph_);
    }
}  // namespace oscm

#endif  // OSCM_COMPUTE_ORDERING_FASTER_H
