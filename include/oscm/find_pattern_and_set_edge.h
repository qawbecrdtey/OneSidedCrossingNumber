#ifndef ONESIDEDCROSSINGNUMBER_OSCM_FIND_PATTERN_0_J_H
#define ONESIDEDCROSSINGNUMBER_OSCM_FIND_PATTERN_0_J_H

#include <oscm/count_crossings.h>
#include <oscm/equal_neighbor.h>

#include <cassert>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <vector>

namespace oscm {
    /**
     *
     * @param nA_
     * @param nB_
     * @param edges_
     * @param directed_edges_ will be updated to contain directed edges from B to B, labeled as v - nA_.
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
                if(!Cij) { directed_edges_[i - nA_].push_back(j - nA_); }
                else if(!Cji) { directed_edges_[j - nA_].push_back(i - nA_); }
            }
        }
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_FIND_PATTERN_0_J_H
