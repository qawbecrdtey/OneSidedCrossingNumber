#ifndef ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_ALL_INTERVALS_H
#define ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_ALL_INTERVALS_H

#include <cstdint>
#include <vector>

namespace oscm {
    __attribute__((flatten))
    std::vector<std::pair<std::uint32_t, std::uint32_t>> obtain_all_intervals(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        std::uint32_t l = 0;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> result;
        for(auto const fixed_vertex: fixed_points_) {
            std::uint32_t j = l;
            while(topological_ordering_[j] != fixed_vertex) { j++; }
            if(l + 1 < j) { result.emplace_back(l, j); }
            l = j + 1;
        }
        if(l + 1 < topological_ordering_.size()) {
            result.emplace_back(l, topological_ordering_.size());
        }
        return result;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_ALL_INTERVALS_H
