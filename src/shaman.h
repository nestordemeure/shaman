#ifndef SHAMAN_H
#define SHAMAN_H

// to get M_PI on windows compiler
#define _USE_MATH_DEFINES

#include <string>
#include <limits>
#include <atomic>
#include <memory>
#include <cmath>
#include <type_traits>

#ifdef SHAMAN_TAGGED_ERROR
#include <shaman/tagged/error_sum.h>
#else
#define FUNCTION_BLOCK
#define LOCAL_BLOCK(text)
#endif

// constexpr that is only used if we are in C++14 or more
#if __cpp_constexpr >= 201304
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14
#endif

//-------------------------------------------------------------------------------------------------
// SHAMAN CLASS

/*
 * the base SHAMAN class, represents a number and its error
 */
template<typename numberType, typename errorType, typename preciseType> class S
{
public:
    using NumberType = numberType;

    // true number ≈ number + errorComposants
    numberType number; // current computed number
    errorType error; // approximation of the current error

#ifdef SHAMAN_TAGGED_ERROR
    error_sum<errorType> errorComposants; // composants of the error
    // base constructors
    inline constexpr S(): number(), error(), errorComposants() {};
    inline constexpr S(numberType numberArg): number(numberArg), error(), errorComposants() {}; // we accept implicit cast from T to S<T>
    inline CONSTEXPR14 S(numberType numberArg, errorType errorArg, error_sum<errorType> errorCompArg): number(numberArg), error(errorArg), errorComposants(errorCompArg)
    {
        #ifdef SHAMAN_FLUSH_NANINF
        if(not std::isfinite(errorArg))
        {
            error = errorType();
            errorComposants = error_sum<errorType>();
        }
        #endif
    };
    // from floating point
    template<typename T,
            typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type,
            typename = typename std::enable_if<not std::is_same<T,numberType>::value, T>::type >
    inline CONSTEXPR14 explicit S(T x): number(x), error(), errorComposants()
    {
        const errorType castError = errorType(x - number);
        error = castError;
        errorComposants.addError(castError);
    };
    // from integer
    template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    inline CONSTEXPR14 S(T x): number(x), error(), errorComposants()
    {
        const errorType castError = errorType(preciseType(x) - number);
        error = castError;
        errorComposants.addError(castError);
    };
    // from other S type
    template<typename n, typename e, typename p,
             typename = typename std::enable_if<not std::is_same<n,numberType>::value, n>::type >
    inline CONSTEXPR14 S(const S<n,e,p>& s): number(s.number), error(s.error), errorComposants(s.errorComposants)
    {
        const errorType castError = errorType(s.number - number);
        error += castError;
        errorComposants.addError(castError);
    };
    // from other volatile S type
    template<typename n, typename e, typename p,
             typename = typename std::enable_if<not std::is_same<n,numberType>::value, n>::type >
    inline CONSTEXPR14 S(const volatile S<n,e,p>& s): number(s.number), error(s.error), errorComposants(const_cast<error_sum<e>&>(s.errorComposants))
    {
        const errorType castError = errorType(s.number - number);
        error += castError;
        errorComposants.addError(castError);
    };
#else
    // base constructors
    inline constexpr S(): number(), error() {};
    inline constexpr S(numberType numberArg): number(numberArg), error() {}; // we accept implicit cast from T to S<T>
    inline CONSTEXPR14 S(numberType numberArg, errorType errorArg): number(numberArg), error(errorArg)
    {
        #ifdef SHAMAN_FLUSH_NANINF
        if(not std::isfinite(errorArg))
        {
            error = errorType();
        }
        #endif
    };
    // from floating point
    template<typename T,
            typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type,
            typename = typename std::enable_if<not std::is_same<T,numberType>::value, T>::type >
    inline constexpr explicit S(T x): number(x), error(x - numberType(x)) {};
    // from integer
    template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
    inline constexpr S(T x): number(x), error(preciseType(x) - numberType(x)) {};
    // from other S type
    template<typename n, typename e, typename p,
            typename = typename std::enable_if<not std::is_same<n,numberType>::value, n>::type>
    inline constexpr S(const S<n,e,p>& s): number(s.number), error(s.error + errorType(s.number - numberType(s.number))) {};
    // from other volatile S type
    template<typename n, typename e, typename p,
            typename = typename std::enable_if<not std::is_same<n,numberType>::value, n>::type>
    inline constexpr S(const volatile S<n,e,p>& s): number(s.number), error(s.error + errorType(s.number - numberType(s.number))) {};
#endif

    // casting
    inline explicit operator bool() const { return (bool) number; };
    inline explicit operator short int() const { return (short int) number; };
    inline explicit operator unsigned short int() const { return (unsigned short int) number; };
    inline explicit operator int() const { return (int) number; };
    inline explicit operator unsigned int() const { return (unsigned int) number; };
    inline explicit operator long int() const { return (long int) number; };
    inline explicit operator unsigned long int() const { return (unsigned long int) number; };
    inline explicit operator long long int() const { return (long long int) number; };
    inline explicit operator unsigned long long int() const { return (unsigned long long int) number; };
    inline explicit operator float() const { return (float) number; };
    inline explicit operator double() const { return (double) number; };
    inline explicit operator long double() const { return (long double) number; };

    // arithmetic operators
    S& operator++();
    S& operator--();
    S& operator++(int);
    S& operator--(int);
    S& operator+=(const S& n);
    S& operator-=(const S& n);
    S& operator*=(const S& n);
    S& operator/=(const S& n);

    // methods
    static numberType digits(numberType number, errorType error);
    numberType digits() const;
    preciseType corrected_number() const;
    std::string to_string() const;

    // unstability detection
    static bool non_significant(numberType number, errorType error);
    bool non_significant() const;
    static void checkUnstableBranch(S n1, S n2);
};

// some macro to shorten template notations
#define templated template<typename numberType, typename errorType, typename preciseType>
#define Snum S<numberType,errorType,preciseType>
#define Serror error_sum<errorType>

//-------------------------------------------------------------------------------------------------
// SHAMAN OPERATIONS

// arithmetic operators
templated const Snum operator+(const Snum& n);
templated const Snum operator-(const Snum& n);
templated const Snum operator+(const Snum& n1, const Snum& n2);
templated const Snum operator-(const Snum& n1, const Snum& n2);
templated const Snum operator*(const Snum& n1, const Snum& n2);
templated const Snum operator/(const Snum& n1, const Snum& n2);

// boolean operators
templated bool operator==(const Snum& n1, const Snum& n2);
templated bool operator!=(const Snum& n1, const Snum& n2);
templated bool operator<(const Snum& n1, const Snum& n2);
templated bool operator<=(const Snum& n1, const Snum& n2);
templated bool operator>(const Snum& n1, const Snum& n2);
templated bool operator>=(const Snum& n1, const Snum& n2);

// shaman specific functions
namespace Shaman
{
    static void unstability();
    static void displayUnstableBranches();
}

// streaming operator
templated std::ostream& operator<<(std::ostream& os, const Snum& n);
templated std::istream& operator>>(std::istream& is, Snum& v);

//-------------------------------------------------------------------------------------------------
// TYPES

#ifdef NO_SHAMAN
using Sfloat = float;
using Sdouble = double;
using Slong_double = long double;
#else
using Sfloat = S<float, float, double>;
using Sdouble = S<double, double, long double>;
using Slong_double = S<long double, long double, long double>;
#endif //NO_SHAMAN

//-------------------------------------------------------------------------------------------------
// SOURCE

#include <shaman/methods.h>
#include <shaman/operators.h>
#include <shaman/functions.h>
#include <shaman/traits.h>
#include <shaman/helpers/shaman_complex.h>
#include <shaman/helpers/shaman_openmp.h>

//-------------------------------------------------------------------------------------------------

#undef templated
#undef Snum
#undef Serror

#endif //SHAMAN_H

