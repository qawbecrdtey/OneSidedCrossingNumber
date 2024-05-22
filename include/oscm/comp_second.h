#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COMP_SECOND_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COMP_SECOND_H

#include <cstdint>
#include <utility>

namespace oscm {
    __attribute__((always_inline)) inline bool comp_second(
      std::pair<std::uint32_t, std::uint32_t> const &a_,
      std::pair<std::uint32_t, std::uint32_t> const &b_) noexcept {
        return a_.second < b_.second;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMP_SECOND_H
