#ifndef OSCM_COUNT_CROSSINGS_H
#define OSCM_COUNT_CROSSINGS_H

#include <oscm/comp_second.h>
#include <oscm/segment_tree.h>

#include <algorithm>
#include <cstdint>
#include <vector>

namespace oscm {
    __attribute__((always_inline, flatten)) inline std::uint64_t count_crossings(
      std::uint32_t const n1_,
      std::uint32_t const n2_,
      std::uint32_t const *const v2_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_) {
        segment_tree const tree(n1_);
        std::uint64_t res = 0;
        for(std::uint32_t i = 0; i < n2_; i++) {
            auto lo = std::lower_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, v2_[i]), comp_second);
            auto const hi = std::upper_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, v2_[i]), comp_second);
#if __has_cpp_attribute(assume)
            [[assume(lo <= hi)]];
#endif
            while(lo < hi) {
                res += tree.sum(lo->first + 1, n1_);
                tree.update_increment(lo->first, 1);
                ++lo;
            }
        }
        return res;
    }
}  // namespace oscm

#endif  // OSCM_COUNT_CROSSINGS_H
