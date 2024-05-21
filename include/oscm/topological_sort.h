#ifndef ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H
#define ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

namespace oscm {
    /**
     * Returns one possible configuration of topological sort, given a directed acyclic graph.
     * @param directed_edges_ There is a directed edge from u to v if v is in directed_edges_[u].
     * @return One possible configuration of topological sort.
     */
    __attribute__((always_inline)) inline std::vector<std::uint32_t>
    topological_sort(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        auto const in_deg = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(auto const &vec: directed_edges_) {
            for(auto const v: vec) { in_deg[v]++; }
        }

        std::vector<std::uint32_t> ordering_stack;
        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
            if(!in_deg[i]) { ordering_stack.push_back(i); }
        }

        std::vector<std::uint32_t> result;
        result.reserve(directed_edges_.size());
        while(!ordering_stack.empty()) {
            result.push_back(ordering_stack.back());
            ordering_stack.pop_back();
            for(auto const next: directed_edges_[result.back()]) {
                if(--in_deg[next]) { continue; }
                ordering_stack.push_back(next);
            }
        }

        assert(result.size() == directed_edges_.size());
#if __has_cpp_attribute(assume)
        [[assume(result.size() == directed_edges_.size())]];
#endif

        return result;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H
