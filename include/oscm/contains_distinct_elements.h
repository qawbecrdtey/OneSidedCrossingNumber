#ifndef OSCM_CONTAINS_UNIQUE_ELEMENTS_H
#define OSCM_CONTAINS_UNIQUE_ELEMENTS_H

#include <cstdint>
#include <vector>

namespace oscm {
    template<typename T>
    bool contains_distinct_elements(std::vector<T> const &vec_) {
        for(std::uint32_t i = 0; i < vec_.size(); i++) {
            for(std::uint32_t j = i + 1; j < vec_.size(); j++) {
                if(vec_[i] == vec_[j]) { return false; }
            }
        }
        return true;
    }
}  // namespace oscm

#endif  // OSCM_CONTAINS_UNIQUE_ELEMENTS_H
