#ifndef ONESIDEDCROSSINGNUMBER_OSCM_TRANSITIVE_REDUCTION_H
#define ONESIDEDCROSSINGNUMBER_OSCM_TRANSITIVE_REDUCTION_H

#include <oscm/topological_sort.h>

#include <cstdint>
#include <ranges>
#include <vector>

namespace oscm {
    [[nodiscard]]
    __attribute__((flatten)) inline std::vector<std::vector<std::uint32_t>> transitive_reduction(
      std::vector<std::vector<std::uint32_t>> &directed_edges_,
      std::vector<std::uint32_t> const &topological_ordering_,
      std::unique_ptr<std::uint32_t[]> topological_ordering_inverse_) {
        static std::uint64_t const N = directed_edges_.size();
        static auto const set_union = std::make_unique<bool[]>(N * N);
        std::fill_n(set_union.get(), N * N, false);
        for(std::uint64_t i = 0; i < N * N; i += N + 1) { set_union[i] = true; }

        std::vector<std::vector<std::uint32_t>> result(directed_edges_.size());
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
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_TRANSITIVE_REDUCTION_H
