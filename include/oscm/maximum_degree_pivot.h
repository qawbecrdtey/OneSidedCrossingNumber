#ifndef OSCM_MAXIMUM_DEGREE_PIVOT_H
#define OSCM_MAXIMUM_DEGREE_PIVOT_H

#include <algorithm>
#include <cstdint>
#include <vector>

namespace oscm {
    std::uint32_t maximum_degree_pivot(
      std::uint32_t const left_,
      std::uint32_t const right_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        auto const degree = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(std::uint32_t now = left_; now < directed_edges_.size(); now++) {
            degree[now] += directed_edges_[now].size();
            for(auto const next: directed_edges_[now]) { degree[next]++; }
        }
        auto const result = std::max_element(degree.get() + left_, degree.get() + right_) - degree.get();
#if __has_cpp_attribute(assume)
        [[assume(left_ <= result && result < right_)]];
#endif

        return result;
    }
}  // namespace oscm

#endif  // OSCM_MAXIMUM_DEGREE_PIVOT_H
