#ifndef OSCM_EQUAL_NEIGHBOR_H
#define OSCM_EQUAL_NEIGHBOR_H

#include <oscm/comp_second.h>

#include <algorithm>
#include <cstdint>
#include <unordered_set>
#include <vector>

namespace oscm {
    __attribute__((flatten)) inline bool equal_neighbor(
      std::uint32_t const i_,
      std::uint32_t const j_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_) {
        std::unordered_set<std::uint32_t> set;

        auto lo_i = std::lower_bound(
          connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second);
        auto const hi_i = std::upper_bound(
          connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second);

#if __has_cpp_attribute(assume)
        [[assume(lo_i <= hi_i)]];
#endif

        auto lo_j = std::lower_bound(
          connections_.begin(), connections_.end(), std::make_pair(0u, j_), comp_second);
        auto const hi_j = std::upper_bound(
          connections_.begin(), connections_.end(), std::make_pair(0u, j_), comp_second);

#if __has_cpp_attribute(assume)
        [[assume(lo_j <= hi_j)]];
#endif

        if(hi_i - lo_i != hi_j - lo_j) { return false; }

        while(lo_i < hi_i) {
            set.insert(lo_i->first);
            ++lo_i;
        }
        while(lo_j < hi_j) {
            auto it = set.find(lo_j->first);
            if(it == set.end()) { return false; }
            set.erase(it);
            ++lo_j;
        }
        return true;
    }
}  // namespace oscm

#endif  // OSCM_EQUAL_NEIGHBOR_H
