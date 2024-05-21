#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H

#include <oscm/comp_second.h>
#include <oscm/segment_tree.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

namespace oscm {
    inline std::uint64_t count_crossings(
      std::uint32_t const n1_,
      std::uint32_t const n2_,
      std::uint32_t const *const v2_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_) {
        segment_tree const tree(n1_);
        std::uint64_t res = 0;
        std::uint32_t idx = 0;
        while(idx < n2_) {
            auto lo = std::lower_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, v2_[idx]), comp_second);
            auto const hi = std::upper_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, v2_[idx]), comp_second);
            idx++;
            while(lo < hi) {
                res += tree.sum(lo->first + 1, n1_);
                tree.update_increment(lo->first, 1);
                ++lo;
            }
        }
        return res;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H
