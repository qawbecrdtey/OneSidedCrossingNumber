#ifndef ONESIDEDCROSSINGNUMBER_OSCM_TRANSITIVE_REDUCTION_H
#define ONESIDEDCROSSINGNUMBER_OSCM_TRANSITIVE_REDUCTION_H

#include <oscm/topological_sort.h>

#include <bitset>
#include <cstdint>
#include <ranges>
#include <vector>

namespace oscm {
    [[nodiscard]]
    std::vector<std::vector<std::uint32_t>>
    transitive_reduction(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        auto topological_ordering = topological_sort(directed_edges_);
        std::vector<std::vector<std::uint8_t>> set_union(
          directed_edges_.size(), std::vector<std::uint8_t>(directed_edges_.size()));
        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) { set_union[i][i] = 1; }

        std::vector<std::vector<std::uint32_t>> result(directed_edges_.size());

        for(auto const now: std::ranges::reverse_view(topological_ordering)) {
            for(auto const next: directed_edges_[now]) {
                if(!set_union[now][next]) {
                    result[now].push_back(next);
                    for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
                        set_union[now][i] |= set_union[next][i];
                    }
                }
            }
        }

        return result;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_TRANSITIVE_REDUCTION_H
