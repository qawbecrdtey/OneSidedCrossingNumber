#ifndef ONESIDEDCROSSINGNUMBER_OSCM_RANDOM_UNSIGNED_INTEGER_H
#define ONESIDEDCROSSINGNUMBER_OSCM_RANDOM_UNSIGNED_INTEGER_H

#include <oscm/random_generator.h>

#include <cstdint>
#include <random>

namespace oscm {
    __attribute__((always_inline, flatten)) inline std::uint32_t
    random_unsigned_integer(std::uint32_t const lo, std::uint32_t const hi) {
        std::uniform_int_distribution<std::uint32_t> dist(lo, hi);
        return dist(random_generator());
    }
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_RANDOM_UNSIGNED_INTEGER_H
