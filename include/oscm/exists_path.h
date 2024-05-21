#ifndef ONESIDEDCROSSINGNUMBER_OSCM_EXISTS_PATH_H
#define ONESIDEDCROSSINGNUMBER_OSCM_EXISTS_PATH_H

#include <cstdint>
#include <memory>
#include <vector>

namespace oscm {
    /**
     * Check if there is a path from i_ to j_ in directed_edges_.
     * @param i_ starting vertex of a path. Satisfies 0 <= i_ <= directed_edges_.size().
     * @param j_ ending vertex of a path. Satisfies 0 <= j_ <= directed_edges_.size().
     * @param directed_edges_ underlying graph.
     * @return true if there exists a path, false otherwise.
     */
    __attribute__((always_inline)) inline bool exists_path(
      std::uint32_t const i_,
      std::uint32_t const j_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        if(i_ == j_) { return true; }
        static auto visited = std::make_unique_for_overwrite<bool[]>(directed_edges_.size());
        std::fill(visited.get(), visited.get() + directed_edges_.size(), false);
        std::vector<std::uint32_t> stack;
        stack.push_back(i_);
        while(!stack.empty()) {
            auto const now = stack.back();
            visited[now] = true;
            stack.pop_back();
            for(auto const next: directed_edges_[now]) {
                if(visited[next]) { continue; }
                if(next == j_) { return true; }
                stack.push_back(next);
            }
        }
        return false;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_EXISTS_PATH_H
