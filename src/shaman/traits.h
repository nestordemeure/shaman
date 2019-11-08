#pragma once

#include <limits>

namespace std
{
    // numeric limits
    templated class numeric_limits<Snum>
    {
    public:
        // Member constants
        static const bool is_specialized = std::numeric_limits<numberType>::is_specialized;
        static const bool is_signed = std::numeric_limits<numberType>::is_signed;
        static const bool is_integer = std::numeric_limits<numberType>::is_integer;
        static const bool is_exact = std::numeric_limits<numberType>::is_exact;
        static const bool has_infinity = std::numeric_limits<numberType>::has_infinity;
        static const bool has_quiet_NaN = std::numeric_limits<numberType>::has_quiet_NaN;
        static const bool has_signaling_NaN = std::numeric_limits<numberType>::has_signaling_NaN;
        static const std::float_denorm_style has_denorm = std::numeric_limits<numberType>::has_denorm;
        static const bool has_denorm_loss = std::numeric_limits<numberType>::has_denorm_loss;
        static const std::float_round_style round_style = std::numeric_limits<numberType>::round_style;
        static const bool is_iec559 = std::numeric_limits<numberType>::is_iec559;
        static const bool is_bounded = std::numeric_limits<numberType>::is_bounded;
        static const bool is_modulo = std::numeric_limits<numberType>::is_modulo;
        static const int digits = std::numeric_limits<numberType>::digits;
        static const int digits10 = std::numeric_limits<numberType>::digits10;
        static const int max_digits10 = std::numeric_limits<numberType>::max_digits10;
        static const int radix = std::numeric_limits<numberType>::radix;
        static const int min_exponent = std::numeric_limits<numberType>::min_exponent;
        static const int min_exponent10 = std::numeric_limits<numberType>::min_exponent10;
        static const int max_exponent = std::numeric_limits<numberType>::max_exponent;
        static const int max_exponent10 = std::numeric_limits<numberType>::max_exponent10;
        static const bool traps = std::numeric_limits<numberType>::traps;
        static const bool tinyness_before = std::numeric_limits<numberType>::tinyness_before;

        // Member functions
        static Snum min() {return Snum(std::numeric_limits<numberType>::min());};
        static Snum lowest() {return Snum(std::numeric_limits<numberType>::lowest());};
        static Snum max() {return Snum(std::numeric_limits<numberType>::max());};
        static Snum epsilon() {return Snum(std::numeric_limits<numberType>::epsilon());};
        static Snum round_error() {return Snum(std::numeric_limits<numberType>::round_error());};
        static Snum infinity() {return Snum(std::numeric_limits<numberType>::infinity());};
        static Snum quiet_NaN() {return Snum(std::numeric_limits<numberType>::quiet_NaN());};
        static Snum signaling_NaN() {return Snum(std::numeric_limits<numberType>::signaling_NaN());};
        static Snum denorm_min() {return Snum(std::numeric_limits<numberType>::denorm_min());};
    };

    // type traits
    templated struct is_floating_point<Snum> : std::is_floating_point<numberType> {};
    templated struct is_arithmetic<Snum> : std::is_arithmetic<numberType> {};
    templated struct is_signed<Snum> : std::is_signed<numberType> {};
}

