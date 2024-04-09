#ifndef ONESIDEDCROSSINGNUMBER_COMPUTE_ORDERING_H
#define ONESIDEDCROSSINGNUMBER_COMPUTE_ORDERING_H

#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    void compute_ordering(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &edges_,
      std::vector<std::uint32_t> &ordering_,
      std::uint32_t const crossing_uppper_bound) {

    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_COMPUTE_ORDERING_H
