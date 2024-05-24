#ifndef ONESIDEDCROSSINGNUMBER_OSCM_C_STORAGE_H
#define ONESIDEDCROSSINGNUMBER_OSCM_C_STORAGE_H

#include <cassert>
#include <cstdint>
#include <vector>

namespace oscm {

    namespace internal {
        static std::vector<bool> C;
        static std::uint64_t N;
    }  // namespace internal

    __attribute__((always_inline, flatten)) inline void C_initialize(std::uint64_t const nB_) {
        internal::C = std::vector<bool>(nB_ * nB_);
        internal::N = nB_;
    }

    __attribute__((always_inline, flatten)) inline void
    C_set(std::uint32_t const i_, std::uint32_t const j_) {
        assert(i_ < C.size() && j_ < C.size());
        internal::C[i_ * internal::N + j_] = true;
        internal::C[j_ * internal::N + i_] = false;
    }

    __attribute__((always_inline, flatten)) inline bool
    C_get(std::uint32_t const i_, std::uint32_t const j_) {
        return internal::C[i_ * internal::N + j_];
    }

}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_C_STORAGE_H
