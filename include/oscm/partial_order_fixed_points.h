#ifndef OSCM_PARTIAL_ORDER_FIXED_POINTS_H
#define OSCM_PARTIAL_ORDER_FIXED_POINTS_H

#include <oscm/is_directed_acyclic.h>
#include <oscm/reverse_directed_edges.h>
#include <oscm/topological_sort.h>

#include <cassert>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace oscm {
    __attribute__((flatten)) inline std::vector<std::uint32_t>  // vector of vertices
    partial_order_fixed_points(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        assert(is_directed_acyclic(directed_edges_));

        std::uint32_t const vertices_count = directed_edges_.size();
        auto const topological_ordering = topological_sort(directed_edges_);
        auto const inverse_map = std::make_unique_for_overwrite<std::uint32_t[]>(vertices_count);

        for(std::uint32_t i = 0; i < vertices_count; i++) {
            inverse_map[topological_ordering[i]] = i;
        }

        auto const reversed_directed_edges = reverse_directed_edges(directed_edges_);
        auto const invalid_range = std::make_unique<std::int32_t[]>(vertices_count + 1);

        for(std::uint32_t v = 0; v < vertices_count; v++) {  // index of vertex
            std::int32_t idx = -1;  // index of topological_ordering
            for(auto const next: reversed_directed_edges[v]) {
                if(idx < static_cast<std::int32_t>(inverse_map[next])) {
                    idx = static_cast<std::int32_t>(inverse_map[next]);
                }
            }
            if(idx + 1 < static_cast<std::int32_t>(inverse_map[v])) {
                invalid_range[idx + 1]++;
                invalid_range[inverse_map[v]]--;
            }

            idx = static_cast<std::int32_t>(vertices_count);
            for(auto const next: directed_edges_[v]) {
                if(idx > static_cast<std::int32_t>(inverse_map[next])) {
                    idx = static_cast<std::int32_t>(inverse_map[next]);
                }
            }
            if(static_cast<std::int32_t>(inverse_map[v] + 1) < idx) {
                invalid_range[inverse_map[v] + 1]++;
                invalid_range[idx]--;
            }
        }

        std::vector<std::uint32_t> result;

        std::int32_t sum = 0;
        for(std::uint32_t i = 0; i < vertices_count; i++) {
            sum += invalid_range[i];
            if(!sum) { result.push_back(topological_ordering[i]); }
        }

        assert(result.size() != vertices_count - 1);
#if __has_cpp_attribute(assume)
        [[assume(result.size() != vertices_count - 1)]];
#endif

        return result;
    }

    /**
     * TODO: fill below.
     * @param directed_edges_ underlying graph.
     * @return a tuple (fixed_points, topological_ordering, topological_ordering_inverse) of \p
     * directed_edges_.
     */
    __attribute__((flatten)) inline std::
      tuple<std::vector<std::uint32_t>, std::vector<std::uint32_t>, std::unique_ptr<std::uint32_t[]>>
      partial_order_fixed_points_with_topological_ordering(
        std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        assert(is_directed_acyclic(directed_edges_));

        auto topological_ordering = topological_sort(directed_edges_);
        auto const vertices_count = static_cast<std::uint32_t>(directed_edges_.size());
        auto inverse_map = std::make_unique_for_overwrite<std::uint32_t[]>(vertices_count);

        for(std::uint32_t i = 0; i < vertices_count; i++) {
            inverse_map[topological_ordering[i]] = i;
        }

        auto const reversed_directed_edges = reverse_directed_edges(directed_edges_);
        auto const invalid_range = std::make_unique<std::int32_t[]>(vertices_count + 1);

        for(std::uint32_t v = 0; v < vertices_count; v++) {  // index of vertex
            std::int32_t idx = -1;  // index of topological_ordering
            for(auto const next: reversed_directed_edges[v]) {
                if(idx < static_cast<std::int32_t>(inverse_map[next])) {
                    idx = static_cast<std::int32_t>(inverse_map[next]);
                }
            }
            if(idx + 1 < static_cast<std::int32_t>(inverse_map[v])) {
                invalid_range[idx + 1]++;
                invalid_range[inverse_map[v]]--;
            }

            idx = static_cast<std::int32_t>(vertices_count);
            for(auto const next: directed_edges_[v]) {
                if(idx > static_cast<std::int32_t>(inverse_map[next])) {
                    idx = static_cast<std::int32_t>(inverse_map[next]);
                }
            }
            if(static_cast<std::int32_t>(inverse_map[v] + 1) < idx) {
                invalid_range[inverse_map[v] + 1]++;
                invalid_range[idx]--;
            }
        }

        std::vector<std::uint32_t> result;

        std::int32_t sum = 0;
        for(std::uint32_t i = 0; i < vertices_count; i++) {
            sum += invalid_range[i];
            if(!sum) { result.push_back(topological_ordering[i]); }
        }

        assert(vertices_count == 1 || result.size() != vertices_count - 1);
#if __has_cpp_attribute(assume)
        [[assume(vertices_count == 1 || result.size() != vertices_count - 1)]];
#endif

        return std::make_tuple<
          std::vector<std::uint32_t>,
          std::vector<std::uint32_t>,
          std::unique_ptr<std::uint32_t[]>>(
          std::move(result), std::move(topological_ordering), std::move(inverse_map));
    }

    /**
     * TODO: fill above.
     * @param directed_edges_ underlying graph.
     * @param B_ an array of length \p nB_ . Works as a map from (0, \p nB_ - 1) to vertices
     * represented as integers from 0 to \p nB_-nA_-1 .
     * @return a tuple (fixed_points, topological_ordering, topological_ordering_inverse) of \p
     * directed_edges_ .
     */
    __attribute__((always_inline, flatten)) inline std::
      tuple<std::vector<std::uint32_t>, std::vector<std::uint32_t>, std::unique_ptr<std::uint32_t[]>>
      partial_order_fixed_points_with_topological_ordering(
        std::uint32_t const nB_,
        std::uint32_t const *const B_,
        std::unordered_map<std::uint32_t, std::uint32_t> const &map_from_vertices_to_B_,
        std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        assert(is_directed_acyclic(directed_edges_));
#if __has_cpp_attribute(assume)
        [[assume(is_directed_acyclic(directed_edges_))]];
#endif

        auto topological_ordering =
          topological_sort(nB_, B_, map_from_vertices_to_B_, directed_edges_);
        auto inverse_map = std::make_unique_for_overwrite<std::uint32_t[]>(nB_);

        for(std::uint32_t i = 0; i < nB_; i++) { inverse_map[topological_ordering[i]] = i; }

        auto const reversed_directed_edges = reverse_directed_edges(directed_edges_);
        auto const invalid_range = std::make_unique<std::int32_t[]>(nB_ + 1);

        for(std::uint32_t v = 0; v < nB_; v++) {
            std::int32_t idx = -1;
            for(auto const next: reversed_directed_edges[B_[v]]) {
                if(auto const it = map_from_vertices_to_B_.find(next);
                   it != map_from_vertices_to_B_.end()) {
                    if(idx < static_cast<std::int32_t>(inverse_map[it->second])) {
                        idx = static_cast<std::int32_t>(inverse_map[it->second]);
                    }
                }
                if(idx + 1 < static_cast<std::int32_t>(inverse_map[B_[v]])) {
                    invalid_range[idx + 1]++;
                    invalid_range[inverse_map[B_[v]]]--;
                }

                idx = static_cast<std::int32_t>(nB_);
                for(auto const next: directed_edges_[B_[v]]) {
                    if(auto const it = map_from_vertices_to_B_.find(next);
                       it != map_from_vertices_to_B_.end()) {
                        if(idx > static_cast<std::int32_t>(inverse_map[it->second])) {
                            idx = static_cast<std::int32_t>(inverse_map[it->second]);
                        }
                    }
                }
                if(static_cast<std::int32_t>(inverse_map[B_[v]] + 1) < idx) {
                    invalid_range[inverse_map[B_[v]] + 1]++;
                    invalid_range[idx]++;
                }
            }

            std::vector<std::uint32_t> result;
            std::int32_t sum = 0;
            for(std::uint32_t i = 0; i < nB_; i++) {
                sum += invalid_range[i];
                if(!sum) { result.push_back(topological_ordering[i]); }
            }

            assert(nB_ == 1 || result.size() != nB_ - 1);
#if __has_cpp_attribute(assume)
            [[assume(nB_ == 1 || result.size() != nB_ - 1)]];
#endif

            return std::make_tuple<
              std::vector<std::uint32_t>,
              std::vector<std::uint32_t>,
              std::unique_ptr<std::uint32_t[]>>(
              std::move(result), std::move(topological_ordering), std::move(inverse_map));
        }
    }
}  // namespace oscm

#endif  // OSCM_PARTIAL_ORDER_FIXED_POINTS_H
