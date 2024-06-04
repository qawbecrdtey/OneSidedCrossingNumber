#ifndef OSCM_CONTAINS_EQUAL_ELEMENTS_H
#define OSCM_CONTAINS_EQUAL_ELEMENTS_H

#include <algorithm>
#include <cstdint>
#include <vector>

namespace oscm {
    template<typename T>
    bool contains_equal_elements(std::vector<T> a_, std::vector<T> b_) {
        if(a_.size() != b_.size()) return false;
        std::sort(a_.begin(), a_.end());
        std::sort(b_.begin(), b_.end());
        for(std::uint64_t i = 0; i < a_.size(); i++) {
            if(a_[i] != b_[i]) { return false; }
        }
        return true;
    }
}  // namespace oscm
#endif  // OSCM_CONTAINS_EQUAL_ELEMENTS_H
