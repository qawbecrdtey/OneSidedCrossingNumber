#ifndef ONESIDEDCROSSINGNUMBER_OSCM_EXISTS_PATH_H
#define ONESIDEDCROSSINGNUMBER_OSCM_EXISTS_PATH_H

#include <cstdint>
#include <vector>

namespace oscm {
    /**
     * Check if there is a path from i_ to j_ in directed_edges_.
     * @param i_ starting vertex of a path. Satisfies 0 <= i_ <= directed_edges_.size().
     * @param j_ ending vertex of a path. Satisfies 0 <= j_ <= directed_edges_.size().
     * @param directed_edges_ underlying graph.
     * @return true if there exists a path, false otherwise.
     */
    inline bool exists_path(
      std::uint32_t i_,
      std::uint32_t j_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        std::vector<std::uint32_t> stack;
        stack.push_back(i_);
        while(!stack.empty()) {
            auto const now = stack.back();
            stack.pop_back();
            if(now == j_) { return true; }
            for(auto const next: directed_edges_[now]) { stack.push_back(next); }
        }
        return false;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_EXISTS_PATH_H
