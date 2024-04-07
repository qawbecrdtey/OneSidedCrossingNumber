#ifndef ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H
#define ONESIDEDCROSSINGNUMBER_OSCM_COUNT_CROSSINGS_H

#include <oscm/segment_tree.h>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

namespace oscm {
    std::uint64_t count_crossings(
      std::uint32_t n1_,
      std::uint32_t n2_,
      std::uint32_t const *v2_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &edges_) {
        static auto const comp =
          [](std::pair<std::uint32_t, std::uint32_t> a_, std::pair<std::uint32_t, std::uint32_t> b_) {
              return a_.second < b_.second;
          };

        segment_tree tree(n1_);
        std::uint64_t res = 0;
        std::uint32_t idx = 0;
        while(idx < n2_) {
            auto lo =
              std::lower_bound(edges_.begin(), edges_.end(), std::make_pair(0u, v2_[idx]), comp);
            auto const hi =
              std::upper_bound(edges_.begin(), edges_.end(), std::make_pair(0u, v2_[idx]), comp);
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
