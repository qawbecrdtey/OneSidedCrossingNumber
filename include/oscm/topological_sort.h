#ifndef ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H
#define ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <queue>
#include <vector>

namespace oscm {
    /**
     * Returns one possible configuration of topological sort, given a directed acyclic graph.
     * @param directed_edges_ There is a directed edge from u to v if v is in directed_edges_[u].
     * @return One possible configuration of topological sort.
     */
    __attribute__((always_inline, flatten)) inline std::vector<std::uint32_t>
    topological_sort(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        auto const in_deg = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(auto const &vec: directed_edges_) {
            for(auto const v: vec) { in_deg[v]++; }
        }

        static std::vector<std::uint32_t> ordering_stack;
        assert(ordering_stack.empty());
#if __has_cpp_attribute(assume)
        [[assume(ordering_stack.empty())]];
#endif

        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
            if(!in_deg[i]) { ordering_stack.push_back(i); }
        }

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

        assert(result.size() == directed_edges_.size());
#if __has_cpp_attribute(assume)
        [[assume(result.size() == directed_edges_.size())]];
#endif

        return result;
    }

    __attribute__((always_inline, flatten)) inline std::vector<std::uint32_t>
    topological_sort_degree_prioritized(
      std::uint32_t const nA_,
      [[maybe_unused]] std::uint32_t const nB_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_) {
        auto const in_deg = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(auto const &vec: directed_edges_) {
            for(auto const v: vec) { in_deg[v]++; }
        }

        auto const degree = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(auto const &[from, to]: connections_) { degree[to - nA_]++; }

        auto const cmp = [&degree](std::uint32_t i_, std::uint32_t j_) {
            return degree[i_] < degree[j_];
        };
        std::priority_queue<std::uint32_t, std::vector<std::uint32_t>, decltype(cmp)> ordering_queue(
          std::move(cmp));

        for(std::uint32_t i = 0; i < directed_edges_.size(); i++) {
            if(!in_deg[i]) { ordering_queue.push(i); }
        }

        std::vector<std::uint32_t> result;
        result.reserve(directed_edges_.size());
        while(!ordering_queue.empty()) {
            result.push_back(ordering_queue.top());
            ordering_queue.pop();
            for(auto const next: directed_edges_[result.back()]) {
#if __has_cpp_attribute(assume)
                [[assume(in_deg[next] > 0)]];
#endif
                if(--in_deg[next]) { continue; }
                ordering_queue.push(next);
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
