#ifndef ONESIDEDCROSSINGNUMBER_OSCM_IS_DIRECTED_ACYCLIC_H
#define ONESIDEDCROSSINGNUMBER_OSCM_IS_DIRECTED_ACYCLIC_H

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>

namespace oscm {
    /**
     * Given a directed graph, returns true if it detects a cycle, false otherwise.
     * @param directed_edges_ Edges of a directed graph.
     * @return true if it detects a cycle, false otherwise.
     */
    bool is_directed_acyclic(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        // TODO: Implement the function.
        return false;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_IS_DIRECTED_ACYCLIC_H
