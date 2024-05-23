#ifndef ONESIDEDCROSSINGNUMBER_OSCM_CUSTOM_PATTERN_H
#define ONESIDEDCROSSINGNUMBER_OSCM_CUSTOM_PATTERN_H

#include <algorithm>
#include <any>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace oscm {
    template<std::uint32_t N>
    bool custom_pattern(
      std::uint32_t i_,
      std::uint32_t j_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::any &result_) {
        result_.reset();

        // a b c d - 1 2
        // a, c - 1
        // b, d - 2
        // result type: bool
        // result_ is true if i -> j, false if j -> i.
        if constexpr(N == 0) {
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

            auto const n = hi_i - lo_i;
            if(lo_i == hi_i || hi_i - lo_i != hi_j - lo_j) { return false; }

            auto const arr =
              std::make_unique_for_overwrite<std::pair<std::uint32_t, std::uint32_t>[]>(n << 1);

            std::uint32_t idx = 0;
            for(auto it = lo_i; it != hi_i; ++it) { arr[idx++] = {it->first, 1}; }
            for(auto it = lo_j; it != hi_j; ++it) { arr[idx++] = {it->first, 0}; }
            std::sort(arr.get(), arr.get() + (n << 1));
            for(std::uint32_t i = 1; i < (n << 1); i++) {
                if(arr[i - 1].first == arr[i].first || arr[i - 1].second != arr[i].second) {
                    return false;
                }
            }
            result_ = std::any(static_cast<bool>(arr[0].second));
            return true;
        }
        return false;
    }

}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_CUSTOM_PATTERN_H
