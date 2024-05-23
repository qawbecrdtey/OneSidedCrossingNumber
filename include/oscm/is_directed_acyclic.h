#ifndef ONESIDEDCROSSINGNUMBER_OSCM_IS_DIRECTED_ACYCLIC_H
#define ONESIDEDCROSSINGNUMBER_OSCM_IS_DIRECTED_ACYCLIC_H

#include <cstdint>
#include <memory>
#include <vector>

namespace oscm {
    /**
     * Given a directed graph, returns true if it detects a cycle, false otherwise.
     * @param directed_edges_ Edges of a directed graph.
     * @return true if it detects a cycle, false otherwise.
     */
    __attribute__((flatten)) inline bool
    is_directed_acyclic(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        auto const in_deg = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(auto const &vec: directed_edges_) {
            for(auto const v: vec) { in_deg[v]++; }
        }

        std::vector<std::uint32_t> ordering_stack;
        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
            if(!in_deg[i]) { ordering_stack.push_back(i); }
        }

#if __has_cpp_attribute(assume)
        [[assume(ordering_stack.size() <= directed_edges_.size())]];
#endif

        std::vector<std::uint32_t> result;
        result.reserve(directed_edges_.size());
        while(!ordering_stack.empty()) {
            result.push_back(ordering_stack.back());
            ordering_stack.pop_back();
            for(auto const next: directed_edges_[result.back()]) {
#if __has_cpp_attribute(assume)
                [[assume(in_deg[next] > 0)]];
#endif
                if(--in_deg[next]) { continue; }
                ordering_stack.push_back(next);
            }
        }

        return ordering_stack.empty();
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_IS_DIRECTED_ACYCLIC_H
