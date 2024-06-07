#ifndef OSCM_BRUTE_FORCE_ORDERING_H
#define OSCM_BRUTE_FORCE_ORDERING_H

#include <oscm/contains_distinct_elements.h>
#include <oscm/count_crossings.h>
#include <oscm/elements_in_range.h>
#include <oscm/exists_reverse_path.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

namespace oscm {
    template<std::uint32_t N>
    void brute_force_ordering(
      std::vector<std::vector<std::uint32_t>> &directed_edges_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> const &topological_ordering_,
      std::uint32_t const nA_,
      std::uint32_t const offset_) {
        std::uint32_t arr[N];
        for(std::uint32_t i = 0; i < N; i++) { arr[i] = topological_ordering_[offset_ + i] + nA_; }
        std::sort(arr, arr + N);
        std::uint32_t res[N];
        auto result_crossing = std::numeric_limits<std::uint64_t>::max();
        do {
            if(exists_reverse_path<N>(arr, nA_, directed_edges_)) continue;
            if(auto const current_crossing = count_crossings(nA_, N, arr, connections_);
               current_crossing < result_crossing) {
                result_crossing = current_crossing;
                std::copy_n(arr, N, res);
            }
        }
        while(std::next_permutation(arr, arr + N));
        for(std::uint32_t i = 1; i < N; i++) {
            directed_edges_[res[i - 1] - nA_].push_back(res[i] - nA_);
        }
    }

    template<std::uint32_t N>
    void brute_force_ordering_modify_topological_ordering(
      std::vector<std::vector<std::uint32_t>> &directed_edges_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_inverse_,
      std::vector<std::uint32_t> &topological_ordering_,
      std::uint32_t const nA_,
      std::uint32_t const offset_) {
        std::uint32_t arr[N];
        for(std::uint32_t i = 0; i < N; i++) {
            assert(offset_ + i < topological_ordering_.size());
            [[assume(offset_ + i < topological_ordering_.size())]];
            arr[i] = directed_edges_index_to_B_map_[topological_ordering_[offset_ + i]] + nA_;
            assert(
              directed_edges_index_to_B_map_inverse_[arr[i] - nA_]
              != std::numeric_limits<std::uint32_t>::max());
        }
        std::sort(arr, arr + N);
        std::uint32_t res[N];
        auto result_crossing = std::numeric_limits<std::uint64_t>::max();
        do {
            if(exists_reverse_path_complex<N>(
                 arr, nA_, directed_edges_index_to_B_map_inverse_, directed_edges_)) {
                continue;
            }
            if(auto const current_crossing = count_crossings(nA_, N, arr, connections_);
               current_crossing < result_crossing) {
                result_crossing = current_crossing;
                std::copy_n(arr, N, res);
            }
        }
        while(std::next_permutation(arr, arr + N));
        for(std::uint32_t i = 1; i < N; i++) {
            directed_edges_[directed_edges_index_to_B_map_inverse_[res[i - 1] - nA_]].push_back(
              directed_edges_index_to_B_map_inverse_[res[i] - nA_]);
        }
        for(std::uint32_t i = 0; i < N; i++) {
            topological_ordering_[offset_ + i] = directed_edges_index_to_B_map_inverse_[res[i] - nA_];
        }
        assert(contains_distinct_elements(topological_ordering_));
        assert(elements_in_range(topological_ordering_, 0, topological_ordering_.size()));
    }
}  // namespace oscm

#endif  // OSCM_BRUTE_FORCE_ORDERING_H
