#ifndef OSCM_EXISTS_REVERSE_PATH_H
#define OSCM_EXISTS_REVERSE_PATH_H

#include <oscm/exists_path.h>

#include <cstdint>
#include <vector>

namespace oscm {
    template<std::uint32_t N>
    bool exists_reverse_path(
      std::uint32_t const *const arr_,
      std::uint32_t const nA_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        for(std::uint32_t i = 1; i < N; i++) {
            for(std::uint32_t j = 0; j < i; j++) {
                if(exists_path(arr_[i] - nA_, arr_[j] - nA_, directed_edges_)) { return true; }
            }
        }
        return false;
    }

    template<std::uint32_t N>
    bool exists_reverse_path_complex(
      std::uint32_t const *const arr_,
      std::uint32_t const nA_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_inverse_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        for(std::uint32_t i = 1; i < N; i++) {
            for(std::uint32_t j = 0; j < i; j++) {
                if(exists_path(
                     directed_edges_index_to_B_map_inverse_[arr_[i] - nA_],
                     directed_edges_index_to_B_map_inverse_[arr_[j] - nA_],
                     directed_edges_)) {
                    return true;
                }
            }
        }
        return false;
    }
}  // namespace oscm

#endif  // OSCM_EXISTS_REVERSE_PATH_H
