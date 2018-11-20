#ifndef SHAMAN_H
#define SHAMAN_H

#include <string>
#include <limits>
#include "src/Debugger.h"

//-------------------------------------------------------------------------------------------------
// SHAMAN CLASS

/*
 * the base SHAMAN class, represents a number and its error
 */
template<typename numberType, typename errorType, typename preciseType> class S
{
public :
    // true number ≈ number + error
    numberType number; // current computed number
    errorType error; // current error
    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
        bool isNumericalZero;
    #endif
    #ifdef DOUBT_LEVEL_FIELD_ENABLED
        int doubtLevel;
    #endif

    // base constructors
    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
        #ifdef DOUBT_LEVEL_FIELD_ENABLED
            inline S(numberType numberArg, errorType errorArg, bool isNumericalZeroArg, int doubt): number(numberArg), error(errorArg), isNumericalZero(isNumericalZeroArg), doubtLevel(doubt) {};
            inline S(numberType numberArg, errorType errorArg, bool isNumericalZeroArg): S(numberArg, errorArg, isNumericalZeroArg, 0) {};
        #else
            inline S(numberType numberArg, errorType errorArg, bool isNumericalZeroArg): number(numberArg), error(errorArg), isNumericalZero(isNumericalZeroArg) {};
        #endif
        inline S(numberType numberArg, errorType errorArg): S(numberArg, errorArg, non_significant(numberArg, errorArg)) {};
        inline S(numberType numberArg): S(numberArg,0,false) {}; // we accept implicit cast from T to S<T>
    #else
        inline S(numberType numberArg, errorType errorArg): number(numberArg), error(errorArg) {};
        inline S(numberType numberArg): S(numberArg,0) {}; // we accept implicit cast from T to S<T>
    #endif
    inline S(): S(0.) {};

    // casting
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
    explicit operator std::string() const;
    #ifdef EXPLICIT_CASTING
    // explicit constructor from other S types (that computes the translation error)
    template<typename n, typename e, typename p>
    inline explicit S(const S<n,e,p>& s): S(s.number, s.error + ((preciseType)s.number - (numberType)s.number)) {};
    // no direct constructor from other types
    template<typename T> S(T s) = delete;
    #else
    // TODO the conversion error could be computed only if the incoming type is less precise than the current type
    // constructor for s types (that computes the translation error)
    template<typename n, typename e, typename p>
    inline S(const S<n,e,p>& s): S(s.number, s.error + ((preciseType)s.number - (numberType)s.number)) {};
    // general constructor (for numeric types that are not numbertype)
    template <typename T, typename = typename std::enable_if<!std::is_same< numberType, T>::value >::type, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    inline S(T n): S((numberType)n, (preciseType)n - (numberType)n) {};
    #endif

    // arithmetic operators
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

    // unstability detection
    static bool non_significant(numberType number, errorType error);
    bool non_significant() const;
    static S minPrecision(const S& n1, const S& n2);
    static bool isCancelation(const S& n, numberType result, errorType resultingError);
    static bool isUnstableBranchings(const S& n1, const S& n2);
};

//-------------------------------------------------------------------------------------------------
// SHAMAN OPERATIONS

// some macro to shorten template notations
#define templated template<typename numberType, typename errorType, typename preciseType>
#define Snum S<numberType,errorType,preciseType>

// streaming operator
templated std::ostream& operator<<(std::ostream& os, const Snum& n);
templated std::istream& operator>>(std::istream& is, Snum& v);

// arithmetic operators
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

// mathematical functions
#ifdef NO_SHAMAN
namespace Sstd = std;
#else
namespace Sstd
{
    templated const Snum sqrt(const Snum& n);
    templated const Snum cbrt(const Snum& n);
    templated const Snum pow(const Snum& n1, const Snum& n2);
    templated const Snum exp(const Snum& n);
    templated const Snum exp2(const Snum& n);
    templated const Snum frexp(const Snum& n, int* exp);
    templated const Snum ldexp(const Snum& n, int exp);
    templated const Snum log(const Snum& n);
    templated const Snum log2(const Snum& n);
    templated const Snum log10(const Snum& n);
    templated const Snum sin(const Snum& n);
    templated const Snum cos(const Snum& n);
    templated const Snum tan(const Snum& n);
    templated const Snum asin(const Snum& n);
    templated const Snum acos(const Snum& n);
    templated const Snum atan(const Snum& n);
    templated const Snum atan2(const Snum& n1, const Snum& n2);
    templated const Snum sinh(const Snum& n);
    templated const Snum cosh(const Snum& n);
    templated const Snum tanh(const Snum& n);
    templated const Snum asinh(const Snum& n);
    templated const Snum acosh(const Snum& n);
    templated const Snum atanh(const Snum& n);
    templated const Snum erf(const Snum& n);
    templated const Snum erfc(const Snum& n);
    templated const Snum abs(const Snum& n);
    templated const Snum fabs(const Snum& n);
    templated const Snum floor(const Snum& n);
    templated const Snum ceil(const Snum& n);
    templated const Snum trunc(const Snum& n);
    templated const Snum min(const Snum& n1, const Snum& n2);
    templated const Snum max(const Snum& n1, const Snum& n2);
    templated const Snum hypot(const Snum& n1, const Snum& n2);
    templated const Snum hypot(const Snum& n1, const Snum& n2, const Snum& n3);
    templated const Snum fma(const Snum& n1, const Snum& n2, const Snum& n3);
    templated bool isfinite(const Snum& n);
    templated bool isnan(const Snum& n);
    using namespace std;
}
#endif

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
// OPENMP (require openMP 4.0+ to get reductions on user defined types)

#ifdef _OPENMP

// +
#pragma omp declare reduction(+:Sfloat : omp_out=omp_in+omp_out)                initializer(omp_priv=Sfloat(0.f))
#pragma omp declare reduction(+:Sdouble: omp_out=omp_in+omp_out)	            initializer(omp_priv=Sdouble(0.))
#pragma omp declare reduction(+:Slong_double: omp_out=omp_in+omp_out)	        initializer(omp_priv=Slong_double(0.L))

// -
#pragma omp declare reduction(-:Sfloat : omp_out=omp_in+omp_out)	            initializer(omp_priv=Sfloat(0.f))
#pragma omp declare reduction(-:Sdouble: omp_out=omp_in+omp_out)	            initializer(omp_priv=Sdouble(0.))
#pragma omp declare reduction(-:Slong_double: omp_out=omp_in+omp_out)	        initializer(omp_priv=Slong_double(0.L))

// *
#pragma omp declare reduction(*:Sfloat : omp_out=omp_in*omp_out)	            initializer(omp_priv=Sfloat(1.f))
#pragma omp declare reduction(*:Sdouble: omp_out=omp_in*omp_out)    	        initializer(omp_priv=Sdouble(1.))
#pragma omp declare reduction(*:Slong_double: omp_out=omp_in*omp_out)	        initializer(omp_priv=Slong_double(1.L))

// max
#pragma omp declare reduction(max:Sfloat : omp_out=max(omp_in,omp_out))	        initializer(omp_priv=Sfloat(std::numeric_limits<float>::lowest()))
#pragma omp declare reduction(max:Sdouble : omp_out=max(omp_in,omp_out))        initializer(omp_priv=Sdouble(std::numeric_limits<double>::lowest()))
#pragma omp declare reduction(max:Slong_double : omp_out=max(omp_in,omp_out))   initializer(omp_priv=Slong_double(std::numeric_limits<long double>::lowest()))

// min
#pragma omp declare reduction(min:Sfloat : omp_out=min(omp_in,omp_out))	        initializer(omp_priv=Sfloat(std::numeric_limits<float>::max()))
#pragma omp declare reduction(min:Sdouble : omp_out=min(omp_in,omp_out))        initializer(omp_priv=Sdouble(std::numeric_limits<double>::max()))
#pragma omp declare reduction(min:Slong_double : omp_out=min(omp_in,omp_out))   initializer(omp_priv=Slong_double(std::numeric_limits<long double>::max()))

#endif //_OPENMP

//-------------------------------------------------------------------------------------------------
// SOURCE

#include "src/Methods.h"
#include "src/Operators.h"
#include "src/Functions.h"

#undef templated
#undef Snum

//-------------------------------------------------------------------------------------------------
#endif //SHAMAN_H