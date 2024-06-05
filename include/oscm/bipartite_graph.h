#ifndef OSCM_BIPARTITE_GRAPH_H
#define OSCM_BIPARTITE_GRAPH_H

#include <oscm/comp_second.h>
#include <oscm/contains_equal_elements.h>
#include <oscm/segment_tree.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

namespace oscm {
    class bipartite_graph {
    public:
        explicit bipartite_graph(
          std::uint32_t const nA_,
          std::vector<std::uint32_t>::iterator side_B_begin_,
          std::vector<std::uint32_t>::iterator side_B_end_,
          std::vector<std::pair<std::uint32_t, std::uint32_t>> const &global_connections_)
         : _nA(nA_),
           _nB(static_cast<std::uint32_t>(side_B_end_ - side_B_begin_)),
           _side_B_begin(side_B_begin_),
           _side_B_end(side_B_end_),
           _global_connections(global_connections_),
           _crossings(0),
           _modified(true) {}

        std::uint64_t count_crossings() const {
            if(!_modified) { return _crossings; }
            _crossings = 0;
            segment_tree tree(_nB);
            for(auto it = _side_B_begin; it != _side_B_end; ++it) {
                auto lo = std::lower_bound(
                  _global_connections.begin(),
                  _global_connections.end(),
                  std::make_pair(0u, *it),
                  comp_second);
                auto const hi = std::upper_bound(
                  _global_connections.begin(),
                  _global_connections.end(),
                  std::make_pair(0u, *it),
                  comp_second);
                [[assume(lo <= hi)]];

                while(lo < hi) {
                    _crossings += tree.sum(lo->first + 1, _nA);
                    tree.update_increment(lo->first, 1);
                    ++lo;
                }
            }
            _modified = false;
            return _crossings;
        }

        void reorder_side_B_with_directed_edges(
          std::vector<std::vector<std::uint32_t>> const &directed_edges_);

    private:
        std::uint32_t const _nA;
        std::uint32_t const _nB;
        std::vector<std::uint32_t>::iterator _side_B_begin;
        std::vector<std::uint32_t>::iterator _side_B_end;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> const &_global_connections;
        mutable std::uint32_t _crossings;
        mutable bool _modified;

        friend void compute_ordering_faster_inner();
    };
}  // namespace oscm

#endif  // OSCM_BIPARTITE_GRAPH_H
