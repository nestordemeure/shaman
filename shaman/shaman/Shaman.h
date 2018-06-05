#ifndef SHAMAN_H
#define SHAMAN_H

#include <string>
#include <limits>
#include "src/Tagger.h"
#include "src/ErrorSum.h"

//-------------------------------------------------------------------------------------------------
// SHAMAN CLASS

/*
 * the base SHAMAN class, represents a number and its errorComposants
 */
template<typename numberType, typename errorType, typename preciseType> class S
{
public :
    // true number ≈ number + errorComposants
    numberType number; // current computed number
    errorType error; // approximation of the current error
    ErrorSum<errorType> errorComposants; // error decomposed per functions TODO could be made optionnal with a flag

    // base constructors
    inline S(numberType numberArg, errorType errorArg, ErrorSum<errorType> errorCompArg): number(numberArg), error(errorArg), errorComposants(errorCompArg) {}; // TODO would a ref improve perfs here ?
    inline S(numberType numberArg): number(numberArg), error(0.), errorComposants() {}; // we accept implicit cast from T to S<T>
    inline S(): number(0.), error(0.), errorComposants() {};

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
        #define EXPLICIT_CAST explicit
        template<typename T> S(T s) = delete;
    #else
        #define EXPLICIT_CAST
    #endif
    #define INTEGER_CAST_CONSTRUCTOR(n) number((numberType)n), error((preciseType)n - (numberType)n), errorComposants("integer_cast", (preciseType)n - (numberType)n)
    template<typename n, typename e, typename p>
    inline EXPLICIT_CAST S(const S<n,e,p>& s): number(s.number), errorComposants(s.errorComposants) {};
    inline EXPLICIT_CAST S(short int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline EXPLICIT_CAST S(unsigned short int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline EXPLICIT_CAST S(int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline EXPLICIT_CAST S(unsigned int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline EXPLICIT_CAST S(long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline EXPLICIT_CAST S(unsigned long int n):INTEGER_CAST_CONSTRUCTOR(n) {};
    inline EXPLICIT_CAST S(long long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline EXPLICIT_CAST S(unsigned long long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    #undef INTEGER_CAST_CONSTRUCTOR
    #undef EXPLICIT_CAST

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
#define Serror ErrorSum<errorType>

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
// TODO the ideal would be able to define std overload for our types
templated const Snum abs(const Snum& n);
templated const Snum fabs(const Snum& n);
templated const Snum sqrt(const Snum& n);
templated const Snum cbrt(const Snum& n);
// TODO templated const Snum pow(const Snum& n1, const Snum& n2);
templated const Snum exp(const Snum& n);
templated const Snum exp2(const Snum& n);
// TODO templated const Snum frexp(const Snum& n, int* exp);
// TODO templated const Snum ldexp(const Snum& n, int exp);
templated const Snum log(const Snum& n);
templated const Snum log2(const Snum& n);
templated const Snum log10(const Snum& n);
templated const Snum sin(const Snum& n);
templated const Snum cos(const Snum& n);
templated const Snum tan(const Snum& n);
templated const Snum asin(const Snum& n);
templated const Snum acos(const Snum& n);
templated const Snum atan(const Snum& n);
// TODO templated const Snum atan2(const Snum& n1, const Snum& n2);
templated const Snum sinh(const Snum& n);
templated const Snum cosh(const Snum& n);
templated const Snum tanh(const Snum& n);
templated const Snum asinh(const Snum& n);
templated const Snum acosh(const Snum& n);
templated const Snum atanh(const Snum& n);
templated const Snum erf(const Snum& n);
templated const Snum floor(const Snum& n);
templated const Snum ceil(const Snum& n);
templated const Snum trunc(const Snum& n);
templated const Snum min(const Snum& n1, const Snum& n2);
templated const Snum max(const Snum& n1, const Snum& n2);
// TODO templated const Snum hypot(const Snum& n1, const Snum& n2);
// TODO templated const Snum hypot(const Snum& n1, const Snum& n2, const Snum& n3);
templated const Snum fma(const Snum& n1, const Snum& n2, const Snum& n3);
templated bool isfinite(const Snum& n);
templated bool isnan(const Snum& n);

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