#ifndef OSCM_ELEMENTS_IN_RANGE_H
#define OSCM_ELEMENTS_IN_RANGE_H

#include <algorithm>
#include <cstdint>
#include <vector>

namespace oscm {
    __attribute__((always_inline)) inline bool elements_in_range(
      std::vector<std::uint32_t> const &vec_,
      std::uint32_t const lo_,
      std::uint32_t const hi_) {
        return std::all_of(vec_.begin(), vec_.end(), [lo_, hi_](std::uint32_t const val_) {
            return lo_ <= val_ && val_ < hi_;
        });
    }
}  // namespace oscm

#endif  // OSCM_ELEMENTS_IN_RANGE_H
