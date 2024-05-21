#ifndef ONESIDEDCROSSINGNUMBER_OSCM_C_STORAGE_H
#define ONESIDEDCROSSINGNUMBER_OSCM_C_STORAGE_H

#include <cassert>
#include <cstdint>
#include <vector>

namespace oscm {
    static std::vector<bool> C;
    static std::uint64_t N;

    __attribute__((always_inline)) inline void C_initialize(std::uint64_t const nB_) {
        C = std::vector<bool>(nB_ * nB_);
        N = nB_;
    }

    __attribute__((always_inline)) inline void C_set(std::uint32_t i_, std::uint32_t j_) {
        assert(i_ < C.size() && j_ < C.size());
        C[i_ * N + j_] = true;
        C[j_ * N + i_] = false;
    }

    __attribute__((always_inline)) inline bool C_get(std::uint32_t i_, std::uint32_t j_) {
        return C[i_ * N + j_];
    }

}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_C_STORAGE_H
