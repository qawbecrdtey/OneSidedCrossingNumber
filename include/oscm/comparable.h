#ifndef OSCM_COMPARABLE_H
#define OSCM_COMPARABLE_H

#include <oscm/exists_path.h>

#include <cstdint>
#include <vector>

namespace oscm {
    /**
     * Check if two vertices \p i_ and \p j_ are comparable.
     * In other words, check if there is some path from \p i_ to \p j_, or vice versa.
     * @param i_ a vertex.
     * @param j_ another vertex.
     * @param directed_edges_ underlying graph.
     * @return true if there is some path from \p i_ to \p j_ or from \p j_ to \p i_, false
     * otherwise.
     */
    __attribute__((always_inline)) inline bool comparable(
      std::uint32_t const i_,
      std::uint32_t const j_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        return exists_path(i_, j_, directed_edges_) || exists_path(j_, i_, directed_edges_);
    }

    /**
     * Check if two vertices \p i_ and \p j_ are not comparable.
     * In other words, check if there are no paths from \p i_ to \p j_ and \p j_ to \p i_.
     * @param i_ a vertex.
     * @param j_ another vertex.
     * @param directed_edges_ underlying graph.
     * @return true if there is no path from \p i_ to \p j_ and from \p j_ to \p i_, false
     * otherwise.
     */
    __attribute__((always_inline, flatten)) inline bool incomparable(
      std::uint32_t const i_,
      std::uint32_t const j_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        return !comparable(i_, j_, directed_edges_);
    }
}  // namespace oscm

#endif  // OSCM_COMPARABLE_H
