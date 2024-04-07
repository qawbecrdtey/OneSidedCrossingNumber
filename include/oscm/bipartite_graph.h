#ifndef ONESIDEDCROSSINGNUMBER_OSCM_GRAPH_H
#define ONESIDEDCROSSINGNUMBER_OSCM_GRAPH_H

#include <algorithm>
#include <cstdint>
#include <memory>
#include <numeric>
#include <vector>

namespace oscm {
    class bipartite_graph {
    public:
        explicit bipartite_graph(
          std::uint32_t nA_,
          std::uint32_t nB_,
          std::vector<std::pair<std::uint32_t, std::uint32_t>> edges_)
         : _vertices_A_count(nA_),
           _vertices_B_count(nB_),
           _vertices_A(std::make_unique_for_overwrite<std::uint32_t[]>(nA_)),
           _vertices_B(std::make_unique_for_overwrite<std::uint32_t[]>(nB_)),
           _edges(std::move(edges_)) {
            std::iota(_vertices_A.get(), _vertices_A.get() + nA_, 1);
            std::iota(_vertices_B.get(), _vertices_B.get() + nB_, nA_ + 1);
            std::sort(_edges.begin(), _edges.end(), [](auto const &a_, auto const &b_) {
                return a_.second < b_.second || (a_.second == b_.second && a_.first < b_.first);
            });
        }

        explicit bipartite_graph(
          std::uint32_t nA_,
          std::uint32_t nB_,
          std::unique_ptr<std::uint32_t[]> vA_,
          std::unique_ptr<std::uint32_t[]> vB_,
          std::vector<std::pair<std::uint32_t, std::uint32_t>> edges_)
         : _vertices_A_count(nA_),
           _vertices_B_count(nB_),
           _vertices_A(std::move(vA_)),
           _vertices_B(std::move(vB_)),
           _edges(std::move(edges_)) {}

    private:
        /** Size of partition A. */
        std::uint32_t _vertices_A_count;
        /** Size of partition B. */
        std::uint32_t _vertices_B_count;
        /** Partition A. */
        std::unique_ptr<std::uint32_t[]> _vertices_A;
        /** Partition B. */
        std::unique_ptr<std::uint32_t[]> _vertices_B;
        /** Sorted in lexicographical order (second, first). */
        std::vector<std::pair<std::uint32_t, std::uint32_t>> _edges;
    };
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_GRAPH_H

