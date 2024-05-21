#ifndef ONESIDEDCROSSINGNUMBER_OSCM_GENERATE_INTERVALS_H
#define ONESIDEDCROSSINGNUMBER_OSCM_GENERATE_INTERVALS_H

#include <algorithm>
#include <cstdint>
#include <random>
#include <vector>

namespace oscm {
    inline std::vector<std::pair<std::uint32_t, std::uint32_t>> generate_intervals(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uint32_t l = 0;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> result;
        for(auto const fixed_vertex: fixed_points_) {
            std::uint32_t j = l;
            while(topological_ordering_[j] != fixed_vertex) { j++; }
            if(j - l > 1) { result.emplace_back(l, j); }
            l = j + 1;
        }
        if(topological_ordering_.size() - l > 1) {
            result.emplace_back(l, topological_ordering_.size());
        }

        std::shuffle(result.begin(), result.end(), gen);

        return result;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_GENERATE_INTERVALS_H