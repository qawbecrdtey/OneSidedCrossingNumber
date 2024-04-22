#ifndef ONESIDEDCROSSINGNUMBER_OSCM_REVERSE_DIRECTED_EDGES_H
#define ONESIDEDCROSSINGNUMBER_OSCM_REVERSE_DIRECTED_EDGES_H

#include <cstdint>
#include <utility>
#include <vector>

namespace oscm {
    std::vector<std::vector<std::uint32_t>>
    reverse_directed_edges(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        std::vector<std::vector<std::uint32_t>> result(directed_edges_.size());
        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
            for(auto const next: directed_edges_[i]) { result[next].push_back(i); }
        }
        return result;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_REVERSE_DIRECTED_EDGES_H
