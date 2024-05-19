#ifndef ONESIDEDCROSSINGNUMBER_OSCM_PARTIAL_ORDER_FIXED_POINTS_H
#define ONESIDEDCROSSINGNUMBER_OSCM_PARTIAL_ORDER_FIXED_POINTS_H

#include <oscm/is_directed_acyclic.h>
#include <oscm/reverse_directed_edges.h>
#include <oscm/topological_sort.h>

#include <cassert>
#include <memory>
#include <vector>

namespace oscm {
    std::vector<std::uint32_t>  // vector of vertices
    partial_order_fixed_points(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        assert(is_directed_acyclic(directed_edges_));

        std::uint32_t const vertices_count = directed_edges_.size();
        auto const topological_ordering = topological_sort(directed_edges_);
        auto const inverse_map = std::make_unique_for_overwrite<std::uint32_t[]>(vertices_count);

        for(std::uint32_t i = 0; i < vertices_count; i++) {
            inverse_map[topological_ordering[i]] = i;
        }

        auto const reversed_direced_edges = reverse_directed_edges(directed_edges_);
        auto const invalid_range = std::make_unique<std::int32_t[]>(vertices_count + 1);

        for(std::uint32_t v = 0; v < vertices_count; v++) {  // index of vertex
            std::uint32_t idx = 0;  // index of topological_ordering
            for(auto const next: reversed_direced_edges[v]) {
                if(idx < inverse_map[next]) { idx = inverse_map[next]; }
            }
            invalid_range[idx + 1]++;
            invalid_range[inverse_map[v]]--;

            idx = vertices_count;
            for(auto const next: directed_edges_[v]) {
                if(idx < inverse_map[next]) { idx = inverse_map[next]; }
            }
            invalid_range[inverse_map[v] + 1]++;
            invalid_range[idx]--;
        }

        std::vector<std::uint32_t> result;

        std::int32_t sum = 0;
        for(std::uint32_t i = 0; i < vertices_count; i++) {
            sum += invalid_range[i];
            if(!sum) { result.push_back(topological_ordering[i]); }
        }

        return result;
    }

    std::pair<std::vector<std::uint32_t>, std::vector<std::uint32_t>>
    partial_order_fixed_points_with_topological_ordering(
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        assert(is_directed_acyclic(directed_edges_));

        std::uint32_t const vertices_count = directed_edges_.size();
        auto topological_ordering = topological_sort(directed_edges_);
        auto const inverse_map = std::make_unique_for_overwrite<std::uint32_t[]>(vertices_count);

        for(std::uint32_t i = 0; i < vertices_count; i++) {
            inverse_map[topological_ordering[i]] = i;
        }

        auto const reversed_direced_edges = reverse_directed_edges(directed_edges_);
        auto const invalid_range = std::make_unique<std::int32_t[]>(vertices_count + 1);

        for(std::uint32_t v = 0; v < vertices_count; v++) {  // index of vertex
            std::uint32_t idx = 0;  // index of topological_ordering
            for(auto const next: reversed_direced_edges[v]) {
                if(idx < inverse_map[next]) { idx = inverse_map[next]; }
            }
            invalid_range[idx + 1]++;
            invalid_range[inverse_map[v]]--;

            idx = vertices_count;
            for(auto const next: directed_edges_[v]) {
                if(idx < inverse_map[next]) { idx = inverse_map[next]; }
            }
            invalid_range[inverse_map[v] + 1]++;
            invalid_range[idx]--;
        }

        std::vector<std::uint32_t> result;

        std::int32_t sum = 0;
        for(std::uint32_t i = 0; i < vertices_count; i++) {
            sum += invalid_range[i];
            if(!sum) { result.push_back(topological_ordering[i]); }
        }

        return {std::move(result), std::move(topological_ordering)};
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_PARTIAL_ORDER_FIXED_POINTS_H
