#ifndef OSCM_REVERSE_DIRECTED_EDGES_H
#define OSCM_REVERSE_DIRECTED_EDGES_H

#include <cstdint>
#include <vector>

namespace oscm {
    __attribute__((always_inline)) inline std::vector<std::vector<std::uint32_t>>
    reverse_directed_edges(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        std::vector<std::vector<std::uint32_t>> result(directed_edges_.size());
        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
            for(auto const next: directed_edges_[i]) { result[next].push_back(i); }
        }
        return result;
    }
}  // namespace oscm

#endif  // OSCM_REVERSE_DIRECTED_EDGES_H
