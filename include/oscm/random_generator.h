#ifndef OSCM_RANDOM_GENERATOR_H
#define OSCM_RANDOM_GENERATOR_H

#include <random>

namespace oscm {
    __attribute__((always_inline)) inline std::mt19937 &random_generator() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }
}  // namespace oscm

#endif  // OSCM_RANDOM_GENERATOR_H
