#ifndef ONESIDEDCROSSINGNUMBER_OSCM_FIND_PATTERN_AND_SET_EDGE_H
#define ONESIDEDCROSSINGNUMBER_OSCM_FIND_PATTERN_AND_SET_EDGE_H

#include <oscm/comp_second.h>
#include <oscm/count_crossings.h>
#include <oscm/equal_neighbor.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

namespace oscm {
    /**
     * Finds certain pattern and sets a directed edge of B, where a -> b implies a < b.
     * @param nA_ Number of vertices of A.
     * @param nB_ Number of vertices of B.
     * @param edges_ Edges as pair of vertices.
     * @param directed_edges_ will be updated to contain directed edges from B to B, labeled as v -
     * nA_.
     */
    void find_pattern_and_set_edge(
      std::uint32_t const nA_,
      std::uint32_t const nB_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &edges_,
      std::vector<std::vector<std::uint32_t>> &directed_edges_) {
        assert(directed_edges_.size() == nB_);
        auto const m = nA_ + nB_;

        for(std::uint32_t i = nA_; i != m; i++) {
            for(std::uint32_t j = i + 1; j != m; j++) {
                if(equal_neighbor(i, j, edges_)) {
                    directed_edges_[i - nA_].push_back(j - nA_);
                    continue;
                }

                std::uint32_t arr[2] {i, j};
                auto const Cij = count_crossings(nA_, 2, arr, edges_);
                arr[0] = j;
                arr[1] = i;
                auto const Cji = count_crossings(nA_, 2, arr, edges_);
                assert(Cij || Cji);
#if __has_cpp_attribute(assume)
                [[assume(Cij || Cji)]];
#endif

                if(!Cij) {
                    directed_edges_[i - nA_].push_back(j - nA_);
                    continue;
                }
                if(!Cji) {
                    directed_edges_[j - nA_].push_back(i - nA_);
                    continue;
                }

                if(Cij == 1 && Cji == 2) {
                    auto const it_lo_i = std::lower_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, i), comp_second);
                    auto const it_hi_i = std::upper_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, i), comp_second);
                    if(it_hi_i - it_lo_i != 2) { continue; }
                    auto const it_lo_j = std::lower_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, j), comp_second);
                    auto const it_hi_j = std::upper_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, j), comp_second);
                    if(it_hi_j - it_lo_j != 2) { continue; }

                    directed_edges_[i - nA_].push_back(j - nA_);
                    continue;
                }

                if(Cij == 2 && Cji == 1) {
                    auto const it_lo_i = std::lower_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, i), comp_second);
                    auto const it_hi_i = std::upper_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, i), comp_second);
                    if(it_hi_i - it_lo_i != 2) { continue; }
                    auto const it_lo_j = std::lower_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, j), comp_second);
                    auto const it_hi_j = std::upper_bound(
                      edges_.begin(), edges_.end(), std::make_pair(0u, j), comp_second);
                    if(it_hi_j - it_lo_j != 2) { continue; }

                    directed_edges_[j - nA_].push_back(i - nA_);
                    continue;
                }
            }
        }
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_FIND_PATTERN_AND_SET_EDGE_H
