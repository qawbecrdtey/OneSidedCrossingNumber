#ifndef ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H
#define ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H

#include <cassert>
#include <cstdint>
#include <memory>
#include <queue>
#include <unordered_map>
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

    /**
     * Returns one possible configuration of topological sort, given a directed acyclic graph.
     * @param nB_
     * @param B_ an array of length \p nB_ . Works as a map from (0, \p nB_ - 1) to vertices
     * represented as integers from 0 to \p nB_-nA_-1 .
     * @param map_from_vertices_to_B_ a hash which is an inverse map of \p B_ .
     * @param directed_edges_ underlying graph.
     * @return a vector of length \p nB_ which works hard.
     */
    __attribute__((always_inline, flatten)) inline std::vector<std::uint32_t> topological_sort(
      std::uint32_t const nB_,
      std::uint32_t const *const B_,
      std::unordered_map<std::uint32_t, std::uint32_t> const &map_from_vertices_to_B_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_) {
        assert(B_ != nullptr);
#if __has_cpp_attribute(assume)
        [[assume(B_ != nullptr)]];
#endif
        auto const in_deg = std::make_unique<std::uint32_t[]>(nB_);
        for(std::uint32_t i = 0; i < nB_; i++) {
            for(auto const v: directed_edges_[B_[i]]) { in_deg[v]++; }
        }

        std::vector<std::uint32_t> ordering_stack;
        for(std::uint32_t i = 0; i < nB_; i++) {
            if(!in_deg[i]) { ordering_stack.push_back(i); }
        }

        std::vector<std::uint32_t> result;
        result.reserve(nB_);
        while(!ordering_stack.empty()) {
            result.push_back(ordering_stack.back());
            ordering_stack.pop_back();
            for(auto const next: directed_edges_[B_[result.back()]]) {
#if __has_cpp_attribute(assume)
                [[assume(in_deg[next] > 0)]];
#endif
                if(map_from_vertices_to_B_.contains(next)) {
                    --in_deg[next];
                    if(!in_deg[next]) { continue; }
                }
                ordering_stack.push_back(next);
            }
        }
        assert(result.size() == nB_);
#if __has_cpp_attribute(assume)
        [[assume(result.size() == nB_)]];
#endif
        return result;
    }

    __attribute__((always_inline, flatten)) inline std::vector<std::uint32_t>
    topological_sort_degree_prioritized(
      std::uint32_t const nA_,
      [[maybe_unused]] std::uint32_t const nB_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_) {
        assert(nB_ == directed_edges_.size());
#if __has_cpp_attribute(assume)
        [[assume(B_ != nullptr)]];
#endif

        auto const in_deg = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(auto const &vec: directed_edges_) {
            for(auto const v: vec) { in_deg[v]++; }
        }

        auto const degree = std::make_unique<std::uint32_t[]>(directed_edges_.size());
        for(auto const &[from, to]: connections_) { degree[to - nA_]++; }

        auto const cmp = [&degree](std::uint32_t const i_, std::uint32_t const j_) {
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

    /**
     * Returns one possible configuration of topological sort with degree prioritized,
     * given a directed acyclic graph.
     * @param nB_
     * @param B_ an array of length \p nB_ . Works as a map from (0, \p nB_ - 1) to vertices
     * represented as integers from 0 to \p nB_-nA_-1 .
     * @param map_from_vertices_to_B_ a hash which is an inverse map of \p B_ .
     * @param directed_edges_ underlying graph.
     * @return a vector of length \p nB_ which works hard.
     */
    __attribute__((always_inline, flatten)) inline std::vector<std::uint32_t>
    topological_sort_degree_prioritized(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::uint32_t const *const B_,
      std::unordered_map<std::uint32_t, std::uint32_t> const &map_from_vertices_to_B_,
      std::vector<std::vector<std::uint32_t>> const &directed_edges_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_) {
        assert(B_ != nullptr);
#if __has_cpp_attribute(assume)
        [[assume(B_ != nullptr)]];
#endif
        auto const in_deg = std::make_unique<std::uint32_t[]>(nB_);
        for(std::uint32_t i = 0; i < nB_; i++) {
            for(auto const v: directed_edges_[B_[i]]) { in_deg[v]++; }
        }

        auto const degree = std::make_unique<std::uint32_t[]>(nB_);
        for(auto const &[from, to]: connections_) { degree[to - nA_]++; }

        auto const cmp = [&degree](std::uint32_t const i_, std::uint32_t const j_) {
            return degree[i_] < degree[j_];
        };
        std::priority_queue<std::uint32_t, std::vector<std::uint32_t>, decltype(cmp)> ordering_queue(
          cmp);

        for(std::uint32_t i = 0; i < nB_; i++) {
            if(!in_deg[i]) { ordering_queue.push(i); }
        }

        std::vector<std::uint32_t> result;
        result.reserve(nB_);
        while(!ordering_queue.empty()) {
            result.push_back(ordering_queue.top());
            ordering_queue.pop();
            for(auto const next: directed_edges_[B_[result.back()]]) {
#if __has_cpp_attribute(assume)
                [[assume(in_deg[next] > 0)]];
#endif
                if(map_from_vertices_to_B_.contains(next)) {
                    --in_deg[next];
                    if(!in_deg[next]) { continue; }
                }
                ordering_queue.push(next);
            }
        }
        assert(result.size() == nB_);
#if __has_cpp_attribute(assume)
        [[assume(result.size() == nB_)]];
#endif
        return result;
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_TOPOLOGICAL_SORT_H
