#ifndef ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H
#define ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H

#include <oscm/obtain_all_intervals.h>
#include <oscm/random_unsigned_integer.h>

#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    __attribute__((always_inline, flatten)) inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval_with_high_priority_on_shorter_interval(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        std::uint32_t l = 0;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> result;
        for(auto const fixed_vertex: fixed_points_) {
            std::uint32_t j = l;
            while(topological_ordering_[j] != fixed_vertex) { j++; }
            // if(l + 2 == j) { return {l, j}; }
            // if(l + 2 < j) { result.emplace_back(l, j); }
            if(l + 2 <= j && j <= l + 6) { return {l, j}; }
            if(l + 6 < j) { result.emplace_back(l, j); }
            l = j + 1;
        }
        if(l + 2 <= topological_ordering_.size() && topological_ordering_.size() <= l + 6) {
            return {l, topological_ordering_.size()};
        }
        if(l + 6 < topological_ordering_.size()) {
            result.emplace_back(l, topological_ordering_.size());
        }
#if __has_cpp_attribute(assume)
        [[assume(!result.empty())]];
#endif
        return *std::min_element(result.begin(), result.end(), [](auto const &a_, auto const &b_) {
#if __has_cpp_attribute(assume)
            [[assume(a_.first <= a_.second)]];
            [[assume(b_.first <= b_.second)]];
#endif
            return a_.second - a_.first < b_.second - b_.first;
        });
    }

    __attribute__((always_inline, flatten)) inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval_with_high_priority_on_longer_interval(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        auto const result = obtain_all_intervals(fixed_points_, topological_ordering_);
        return *std::max_element(result.begin(), result.end(), [](auto const &a_, auto const &b_) {
            return a_.second - a_.first - b_.second - b_.first;
        });
    }

    __attribute__((always_inline, flatten)) inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval_with_high_priority_on_first_interval(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        std::uint32_t l = 0;
        for(auto const fixed_vertex: fixed_points_) {
            std::uint32_t j = l;
            while(topological_ordering_[j] != fixed_vertex) { j++; }
            if(l + 1 < j) { return {l, j}; }
            l = j + 1;
        }
        if(l + 1 < topological_ordering_.size()) { return {l, topological_ordering_.size()}; }
#if __cplusplus >= 202'302L
        std::unreachable();
#else
        __builtin_unreachable();
#endif
    }

    __attribute__((always_inline, flatten)) inline std::pair<std::uint32_t, std::uint32_t>
    obtain_random_interval(
      std::vector<std::uint32_t> const &fixed_points_,
      std::vector<std::uint32_t> const &topological_ordering_) {
        auto const result = obtain_all_intervals(fixed_points_, topological_ordering_);
        return result[random_unsigned_integer(0, result.size() - 1)];
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_OBTAIN_RANDOM_INTERVAL_H