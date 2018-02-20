#ifndef SHAMAN_H
#define SHAMAN_H

#include <string>

// some macro to shorten template notations
#define templated template<typename numberType, typename errorType, typename preciseType>
#define Snum S<numberType,errorType,preciseType>

/*
 * the base SHAMAN class, represents a number and its error
 */
templated class S
{
public :
    // trueNumber = number + error
    numberType number; // current computed number
    errorType error; // current error

    // constructors
    inline S(): number(0), error(0) {};
    inline S(numberType numberArg): number(numberArg), error(0) {}; // we accept implicit cast from T to S<T>
    inline S(numberType numberArg, errorType errorArg): number(numberArg), error(errorArg) {};

    // casting
    inline explicit operator int() const { return (int) number; };
    inline explicit operator float() const { return (float) number; };
    inline explicit operator double() const { return (double) number; };
    inline explicit operator long double() const { return (long double) number; };
    explicit operator std::string() const;
    #ifdef EXPLICIT_CASTING
    // requires explicit cast from other S types
    template<typename n, typename e, typename p>
    inline explicit S(const S<n,e,p>& s): number(s.number), error(s.error) {};
    // refuse cast from types other than numberType
    template<typename T> S(T s) = delete;
    #else
    template<typename n, typename e, typename p>
    inline S(const S<n,e,p>& s): number(s.number), error(s.error) {};
    #endif

    // arithmetic operators
    S& operator+=(const S& n);
    S& operator-=(const S& n);
    S& operator*=(const S& n);
    S& operator/=(const S& n);

    // operations
    static numberType digits(numberType number, errorType error);
    static numberType digits(const S& n);

    // unstability detection
    static bool non_significativ(numberType number, errorType error);
    bool non_significativ() const;
    static S minPrecision(const S& n1, const S& n2);
    static bool isCancelation(const S& n, numberType result, errorType resultingError);
    static bool isUnstableBranchings(const S& n1, const S& n2);
};

// boolean operators
templated bool operator==(const Snum& n1, const Snum& n2);
templated bool operator!=(const Snum& n1, const Snum& n2);
templated bool operator<(const Snum& n1, const Snum& n2);
templated bool operator<=(const Snum& n1, const Snum& n2);
templated bool operator>(const Snum& n1, const Snum& n2);
templated bool operator>=(const Snum& n1, const Snum& n2);

// arithmetic operators
templated const Snum operator-(const Snum& n);
templated const Snum operator+(const Snum& n1, const Snum& n2);
templated const Snum operator-(const Snum& n1, const Snum& n2);
templated const Snum operator*(const Snum& n1, const Snum& n2);
templated const Snum operator/(const Snum& n1, const Snum& n2);

// mathematical functions
templated bool isfinite(const Snum& n);
templated const Snum abs(const Snum& n);
templated const Snum fabs(const Snum& n);
templated const Snum sqrt(const Snum& n);
templated const Snum cbrt(const Snum& n);
templated const Snum exp(const Snum& n);
templated const Snum sin(const Snum& n);
templated const Snum cos(const Snum& n);
templated const Snum tan(const Snum& n);
templated const Snum pow(const Snum& n1, const Snum& n2);
templated const Snum min(const Snum& n1, const Snum& n2);
templated const Snum max(const Snum& n1, const Snum& n2);
templated const Snum fma(const Snum& n1, const Snum& n2, const Snum& n3);

// streaming operator
templated std::ostream& operator<<(std::ostream& os, const Snum& n);

// basic types
#ifdef NO_SHAMAN
using Sfloat = float;
using Sdouble = double;
using Slong_double = long double;
#else
using Sfloat = S<float, float, double>;
using Sdouble = S<double, double, long double>;
using Slong_double = S<long double, long double, long double>;
#endif

/*
// CADNA openMP definition

#ifdef _OPENMP
#pragma omp threadprivate (_cadna_random, _cadna_random_counter, _cadna_recurrence)
#endif //_OPENMP

#ifdef _OPENMP
#pragma omp declare reduction(+:float_st : omp_out=omp_in+omp_out)	\
  initializer(omp_priv=float_st(0.f))
#pragma omp declare reduction(+:double_st: omp_out=omp_in+omp_out)	\
  initializer(omp_priv=double_st(0.))
#pragma omp declare reduction(-:float_st : omp_out=omp_in+omp_out)	\
  initializer(omp_priv=float_st(0.f))
#pragma omp declare reduction(-:double_st: omp_out=omp_in+omp_out)	\
  initializer(omp_priv=double_st(0.))
#pragma omp declare reduction(*:float_st : omp_out=omp_in*omp_out)	\
  initializer(omp_priv=float_st(1.f))
#pragma omp declare reduction(*:double_st: omp_out=omp_in*omp_out)	\
  initializer(omp_priv=double_st(1.))
#endif //_OPENMP
*/

#endif //SHAMAN_H
#include "Shaman_Decl.h"
