#ifndef ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H
#define ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H

#include <oscm/random_unsigned_integer.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    __attribute__((always_inline, flatten)) inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        std::uint32_t l = 0;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> result;
        for(auto const fixed_vertex: fixed_points_) {
            std::uint32_t j = l;
            while(topological_ordering_[j] != fixed_vertex) { j++; }
            // if(l + 2 == j) { return {l, j}; }
            // if(l + 2 < j) { result.emplace_back(l, j); }
            if(l + 2 <= j && j <= l + brute_force_constant) { return {l, j}; }
            if(l + brute_force_constant < j) { result.emplace_back(l, j); }
            l = j + 1;
        }
        // if(l + 2 == topological_ordering_.size()) { return {l, topological_ordering_.size()}; }
        // if(l + 2 < topological_ordering_.size()) {
        //     result.emplace_back(l, topological_ordering_.size());
        // }
        if(
          l + 2 <= topological_ordering_.size()
          && topological_ordering_.size() <= l + brute_force_constant) {
            return {l, topological_ordering_.size()};
        }
        if(l + brute_force_constant < topological_ordering_.size()) {
            result.emplace_back(l, topological_ordering_.size());
        }
#if __has_cpp_attribute(assume)
        [[assume(!result.empty())]];
#endif
        // 1.
        // return result[random_unsigned_integer(0, result.size() - 1)];

        // 2.
        return *std::min_element(result.begin(), result.end(), [](auto const &a_, auto const &b_) {
            return a_.second - a_.first < b_.second - b_.first;
        });
    }

    __attribute__((always_inline, flatten)) inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval_with_high_priority_on_longer_interval(
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
        return *std::max_element(result.begin(), result.end(), [](auto const &a_, auto const &b_) {
            return a_.second - a_.first - b_.second - b_.first;
        });
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H