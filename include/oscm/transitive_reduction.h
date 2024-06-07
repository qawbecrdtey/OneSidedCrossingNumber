#ifndef OSCM_TRANSITIVE_REDUCTION_H
#define OSCM_TRANSITIVE_REDUCTION_H

#include <oscm/topological_sort.h>

#include <algorithm>
#include <cstdint>
#include <ranges>
#include <vector>

namespace oscm {
    [[nodiscard]]
    __attribute__((always_inline, flatten)) inline std::vector<std::vector<std::uint32_t>>
    transitive_reduction(
      std::vector<std::vector<std::uint32_t>> &directed_edges_,
      std::vector<std::uint32_t> const &topological_ordering_,
      std::uint32_t const *topological_ordering_inverse_) {
        std::uint64_t const N = directed_edges_.size();
        auto const set_union = std::make_unique<bool[]>(N * N);
        for(std::uint64_t i = 0; i < N * N; i += N + 1) { set_union[i] = true; }

        std::vector<std::vector<std::uint32_t>> result(N);
        for(auto const now: std::ranges::reverse_view(topological_ordering_)) {
            // TODO: should be optimized.
            std::sort(
              directed_edges_[now].begin(),
              directed_edges_[now].end(),
              [&topological_ordering_inverse_](std::uint32_t const a_, std::uint32_t const b_) {
                  return topological_ordering_inverse_[a_] < topological_ordering_inverse_[b_];
              });
            auto const now_N = now * N;
            for(auto const next: directed_edges_[now]) {
                if(!set_union[now_N + next]) {
                    result[now].push_back(next);
                    auto const next_N = next * N;
                    for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
                        set_union[now_N + i] = set_union[now_N + i] || set_union[next_N + i];
                    }
                }
            }
        }

        return result;
    }

    [[nodiscard]]
    __attribute__((flatten)) inline std::vector<std::vector<std::uint32_t>> transitive_reduction(
      std::vector<std::vector<std::uint32_t>> &directed_edges_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        auto topological_ordering_inverse =
          std::make_unique_for_overwrite<std::uint32_t[]>(topological_ordering_.size());
        for(std::uint32_t i = 0; i < topological_ordering_.size(); i++) {
            topological_ordering_inverse[topological_ordering_[i]] = i;
        }
        return transitive_reduction(
          directed_edges_, topological_ordering_, topological_ordering_inverse.get());
    }
}  // namespace oscm

#endif  // OSCM_TRANSITIVE_REDUCTION_H
