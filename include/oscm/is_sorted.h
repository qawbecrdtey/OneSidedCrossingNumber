#ifndef ONESIDEDCROSSINGNUMBER_OSCM_IS_SORTED_H
#define ONESIDEDCROSSINGNUMBER_OSCM_IS_SORTED_H

#include <cstdint>

namespace oscm {
    __attribute__((always_inline, flatten)) inline bool
    is_sorted(std::uint32_t const n_, std::uint32_t const *const arr_) {
        for(std::uint32_t i = 0; i < n_ - 1; i++) {
            if(arr_[i] > arr_[i + 1]) { return false; }
        }
        return true;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_IS_SORTED_H
