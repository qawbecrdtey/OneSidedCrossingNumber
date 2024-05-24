#ifndef ONESIDEDCROSSINGNUMBER_RELEASE_VECTOR_MEMORY_H
#define ONESIDEDCROSSINGNUMBER_RELEASE_VECTOR_MEMORY_H

#include <vector>

namespace oscm {
    template<typename T>
    void release_vector_memory(std::vector<T> &vec_) {
        std::vector<T>().swap(vec_);
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_RELEASE_VECTOR_MEMORY_H
