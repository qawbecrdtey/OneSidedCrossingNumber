#ifndef ONESIDEDCROSSINGNUMBER_OSCM_PARTIAL_ORDER_FIXED_POINTS_H
#define ONESIDEDCROSSINGNUMBER_OSCM_PARTIAL_ORDER_FIXED_POINTS_H

#include <oscm/topological_sort.h>

#include <vector>

namespace oscm {
    std::vector<std::pair<std::uint32_t, std::uint32_t>>  // index, vertex
    partial_order_fixed_points(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        std::vector<std::pair<std::uint32_t, std::uint32_t>> result;
        auto const topological_ordering = topological_sort(directed_edges_);

        return result;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_PARTIAL_ORDER_FIXED_POINTS_H
