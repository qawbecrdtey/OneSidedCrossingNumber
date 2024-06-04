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
          std::vector<std::uint32_t> side_B_,
          std::vector<std::pair<std::uint32_t, std::uint32_t>> const &global_connections_)
         : _nA(nA_),
           _nB(static_cast<std::uint32_t>(side_B_.size())),
           _side_B(std::move(side_B_)),
           _global_connections(global_connections_),
           _crossings(0),
           _modified(true) {}

        std::uint64_t count_crossings() const {
            if(!_modified) { return _crossings; }
            _crossings = 0;
            segment_tree tree(_nB);
            for(auto const b: _side_B) {
                auto lo = std::lower_bound(
                  _global_connections.begin(),
                  _global_connections.end(),
                  std::make_pair(0u, b),
                  comp_second);
                auto const hi = std::upper_bound(
                  _global_connections.begin(),
                  _global_connections.end(),
                  std::make_pair(0u, b),
                  comp_second);
#if __has_cpp_attribute(assume)
                [[assume(lo <= hi)]];
#endif

                while(lo < hi) {
                    _crossings += tree.sum(lo->first + 1, _nA);
                    tree.update_increment(lo->first, 1);
                    ++lo;
                }
            }
            _modified = false;
            return _crossings;
        }

        void reorder_side_B(std::vector<std::uint32_t> new_side_B_) {
            assert(contains_equal_elements(_side_B, new_side_B_));
#if __has_cpp_attribute(assume)
            [[assume(contains_equal_elements(_side_B, new_side_B_))]];
#endif

            _side_B = std::move(new_side_B_);
            _modified = true;
        }

        void reorder_side_B_with_directed_edges(std::vector<std::vector<std::uint32_t>> const &directed_edges_) {

        }

    private:
        std::uint32_t const _nA;
        std::uint32_t const _nB;
        std::vector<std::uint32_t> _side_B;
        std::vector<std::pair<std::uint32_t, std::uint32_t>> const &_global_connections;
        mutable std::uint32_t _crossings;
        mutable bool _modified;

        friend void compute_ordering_faster_inner();
    };
}  // namespace oscm

#endif  // OSCM_BIPARTITE_GRAPH_H
