#ifndef ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H
#define ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H

#include <oscm/random_unsigned_integer.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    __attribute__((always_inline)) inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        std::uint32_t l = 0;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> result;
        for(auto const fixed_vertex: fixed_points_) {
            std::uint32_t j = l;
            while(topological_ordering_[j] != fixed_vertex) { j++; }
            if(j - l == 2) { return {l, j}; }
            if(j - l > 1) { result.emplace_back(l, j); }
            l = j + 1;
        }
        if(topological_ordering_.size() - l > 1) {
            if(topological_ordering_.size() - l == 2) { return {l, topological_ordering_.size()}; }
            result.emplace_back(l, topological_ordering_.size());
        }
#if __has_cpp_attribute(assume)
        [[assume(!result.empty())]];
#endif

        return result[random_unsigned_integer(0, result.size() - 1)];
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H