#ifndef ONESIDEDCROSSINGNUMBER_OSCM_RATIONAL_H
#define ONESIDEDCROSSINGNUMBER_OSCM_RATIONAL_H

#include <cstdint>
#include <numeric>

namespace oscm {
    class rational {
    public:
        constexpr explicit rational(std::int64_t const integer_)
         : _numerator(integer_),
           _denominator(1) {}

        constexpr explicit rational(std::int64_t const numerator_, std::uint64_t const denominator_)
         : _numerator(numerator_),
           _denominator(denominator_) {
            std::uint64_t const g = std::gcd(_numerator, _denominator);
            _numerator /= g;
            _denominator /= g;
        }



    private:
        std::int64_t _numerator;
        std::uint64_t _denominator;
    };
}  // namespace oscm

#endif  // ONESIDEDCROSSINGNUMBER_OSCM_RATIONAL_H
