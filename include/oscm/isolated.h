#ifndef OSCM_ISOLATED_H
#define OSCM_ISOLATED_H

#include <algorithm>

namespace oscm {
    __attribute__((always_inline, flatten)) inline bool isolated(
      std::uint32_t i_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_) {
        return std::lower_bound(
                 connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second)
            == std::upper_bound(
                 connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second);
    }
}  // namespace oscm

#endif  // OSCM_ISOLATED_H
