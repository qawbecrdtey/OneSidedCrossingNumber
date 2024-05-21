#ifndef ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H
#define ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H

#include <cstdint>
#include <random>
#include <utility>
#include <vector>

namespace oscm {
    inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval(
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
#if __has_cpp_attribute(assume)
        [[assume(!result.empty())]];
#endif

        std::uniform_int_distribution<std::uint32_t> dist(0, result.size() - 1);
        return result[dist(gen)];
    }
}

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H