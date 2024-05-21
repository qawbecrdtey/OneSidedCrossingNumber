#ifndef ONESIDEDCROSSINGNUMBER_OSCM_DEPENDENT_H
#define ONESIDEDCROSSINGNUMBER_OSCM_DEPENDENT_H

#include <oscm/comparable.h>

#include <algorithm>
#include <cstdint>
#include <vector>

namespace oscm {
    /**
     * Checks if \p i_ and \p j_ are dependent.
     * In other words, check if there is some v which (v, \p i_) or (v, \p j_) is incomparable.
     * @param i_ a vertex. Satisfies 0 <= \p i_ <= directed_edges_.size().
     * @param j_ another vertex. Satisfies 0 <= \p j_ <= directed_edges_.size().
     * @param directed_edges_ underlying graph.
     * @return true if \p i_ and \p j_ are dependent, false otherwise.
     */
    inline bool dependent(
      std::uint32_t const i_,
      std::uint32_t const j_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
            if(incomparable(i, i_, directed_edges_) || incomparable(i, j_, directed_edges_)) {
                return false;
            }
        }
        return true;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_DEPENDENT_H
