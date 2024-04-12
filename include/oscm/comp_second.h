#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COMP_SECOND_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COMP_SECOND_H

#include <cstdint>
#include <utility>

namespace oscm {
    bool comp_second(
      std::pair<std::uint32_t, std::uint32_t> a_,
      std::pair<std::uint32_t, std::uint32_t> b_) {
        return a_.second < b_.second;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COMP_SECOND_H
