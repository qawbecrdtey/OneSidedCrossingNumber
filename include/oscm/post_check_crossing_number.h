#ifndef OSCM_POST_CHECK_CROSSING_NUMBER_H
#define OSCM_POST_CHECK_CROSSING_NUMBER_H

#include <oscm/count_crossings.h>

#include <cstdint>
#include <functional>
#include <vector>

namespace oscm {
    inline bool post_check_crossing_number(
      std::uint32_t const nA_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> const &ordering_,
      std::vector<std::uint32_t> const &directed_edges_index_to_B_map_,
      std::uint64_t const crossing_upper_bound_) {
        std::vector<std::uint32_t> subset_B(ordering_.size());
        for(std::uint32_t i = 0; i < ordering_.size(); i++) {
            subset_B[i] = directed_edges_index_to_B_map_[ordering_[i]] + nA_;
        }
        return crossing_upper_bound_
            == count_crossings(nA_, subset_B.size(), subset_B.data(), connections_);
    }

    inline bool post_check_crossing_number(
      std::uint32_t const nA_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::vector<std::uint32_t> const &ordering_,
      std::function<std::uint32_t(std::uint32_t)> const &directed_edges_index_to_B_map_,
      std::uint64_t const crossing_upper_bound_) {
        std::vector<std::uint32_t> subset_B(ordering_.size());
        for(std::uint32_t i = 0; i < ordering_.size(); i++) {
            subset_B[i] = directed_edges_index_to_B_map_(ordering_[i]) + nA_;
        }
        return crossing_upper_bound_
            == count_crossings(nA_, subset_B.size(), subset_B.data(), connections_);
    }
}  // namespace oscm

#endif  // OSCM_POST_CHECK_CROSSING_NUMBER_H
