#ifndef OSCM_CUSTOM_PATTERN_H
#define OSCM_CUSTOM_PATTERN_H

#include <algorithm>
#include <any>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace oscm {
    /**
     * List of patterns which determines an arrow of a directed graph.
     *
     * - N = 0
     *   - 0 1 2 3 ... (2k - 1) - a b
     *   - 0 2 4 ... (2k - 2) - a
     *   - 1 3 5 ... (2k - 1) - b
     *   - result type: bool
     *   - result_ is true if a -> b, false if b -> a.
     *
     * - N = 1
     *   - Check all patterns like RR3
     *   - result type: bool
     *   - result_ is true if a -> b, false if b -> a.
     *
     * @tparam N the index of a pattern.
     * @param i_ from vertex.
     * @param j_ to vertex.
     * @param connections_ the bipartite graph in question.
     * @param result_ a std::any parameter to return additional result.
     * @return true if it matches a pattern N, false otherwise.
     */
    template<std::uint32_t N>
    bool custom_pattern(
      [[maybe_unused]] std::uint32_t const nA_,
      [[maybe_unused]] std::uint32_t const nB_,
      std::uint32_t const i_,
      std::uint32_t const j_,
      std::uint32_t const Cij_,
      std::uint32_t const Cji_,
      std::vector<std::pair<std::uint32_t, std::uint32_t>> const &connections_,
      std::any &result_) {
        if constexpr(N == 0) {
            auto lo_i = std::lower_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second);
            auto const hi_i = std::upper_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second);

#if __has_cpp_attribute(assume)
            [[assume(lo_i <= hi_i)]];
#endif

            auto lo_j = std::lower_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, j_), comp_second);
            auto const hi_j = std::upper_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, j_), comp_second);

#if __has_cpp_attribute(assume)
            [[assume(lo_j <= hi_j)]];
#endif

            auto const n = hi_i - lo_i;
            if(lo_i == hi_i || hi_i - lo_i != hi_j - lo_j) { return false; }

#if __has_cpp_attribute(assume)
            [[assume(hi_i != lo_i)]];
            [[assume(hi_i - lo_i == hi_j - lo_j)]];
            [[assume(n > 0)]];
#endif

            auto const arr =
              std::make_unique_for_overwrite<std::pair<std::uint32_t, std::uint32_t>[]>(n << 1);

            std::uint32_t idx = 0;
            for(auto it = lo_i; it != hi_i; ++it) { arr[idx++] = {it->first, 1}; }
            for(auto it = lo_j; it != hi_j; ++it) { arr[idx++] = {it->first, 0}; }
            std::sort(arr.get(), arr.get() + (n << 1));
            for(std::uint32_t i = 1; i < (n << 1); i++) {
                if(arr[i - 1].first == arr[i].first || arr[i - 1].second == arr[i].second) {
                    return false;
                }
            }
            result_ = std::any(static_cast<bool>(arr[0].second));
            return true;
        }
        if constexpr(N == 1) {
            auto lo_i = std::lower_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second);
            auto const hi_i = std::upper_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, i_), comp_second);

#if __has_cpp_attribute(assume)
            [[assume(lo_i <= hi_i)]];
#endif

            auto lo_j = std::lower_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, j_), comp_second);
            auto const hi_j = std::upper_bound(
              connections_.begin(), connections_.end(), std::make_pair(0u, j_), comp_second);

#if __has_cpp_attribute(assume)
            [[assume(lo_j <= hi_j)]];
#endif

            auto const n = hi_i - lo_i;
            if(lo_i == hi_i || hi_i - lo_i != hi_j - lo_j) { return false; }

#if __has_cpp_attribute(assume)
            [[assume(hi_i != lo_i)]];
            [[assume(hi_i - lo_i == hi_j - lo_j)]];
            [[assume(n > 0)]];
#endif

            std::map<std::uint32_t, std::vector<std::uint32_t>> map;
            for(std::uint32_t i = 0; i < n; i++) { map[(lo_i + i)->first].push_back(0); }
            for(std::uint32_t i = 0; i < n; i++) { map[(lo_j + i)->first].push_back(1); }

            // A : i -> j
            // B : j -> i
            auto it = map.begin();
            std::uint32_t Ai = n;
            std::uint32_t Aj = 0;
            std::uint32_t Bi = 0;
            std::uint32_t Bj = n;
            bool i_to_j = Cij_ < Cji_;
            bool j_to_i = Cji_ < Cij_;
            while(it != map.end()) {
                assert(Ai <= n && Aj <= n && Bi <= n && Bj <= n);
#if __has_cpp_attribute(assume)
                [[assume(!it->second.empty())]];
                [[assume(it->second.size() < 3)]];
                [[assume(Ai <= n)]];
                [[assume(Aj <= n)]];
                [[assume(Bi <= n)]];
                [[assume(Bj <= n)]];
#endif

                for(auto const val: it->second) {
#if __has_cpp_attribute(assume)
                    [[assume(val == 0 || val == 1)]];
#endif
                    if(!val) { Ai--; }
                    else { Bj--; }
                }

                if(Ai + Aj < Bi + Bj) { i_to_j = true; }
                else if(Bi + Bj < Ai + Aj) { j_to_i = true; }
                if(i_to_j && j_to_i) { return false; }

                for(auto const val: it->second) {
#if __has_cpp_attribute(assume)
                    [[assume(val == 0 || val == 1)]];
#endif
                    if(!val) { Bi++; }
                    else { Aj++; }
                }

                auto const next_it = std::next(it);
                if(it->first + 1 != next_it->first) {
                    if(Ai + Aj < Bi + Bj) { i_to_j = true; }
                    else if(Bi + Bj < Ai + Aj) { j_to_i = true; }
                    if(i_to_j && j_to_i) { return false; }
                }

                it = next_it;
            }
#if DEBUG_MESSAGE
            std::cout << "Found pattern " << i_ << (i_to_j ? " -> " : " <- ") << j_ << std::endl;
#endif
            result_ = i_to_j;
            return true;
        }
        return false;
    }

}  // namespace oscm

#endif  // OSCM_CUSTOM_PATTERN_H
