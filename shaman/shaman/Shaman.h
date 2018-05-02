#ifndef SHAMAN_H
#define SHAMAN_H

#include <string>
#include "src/NumericalDebugger.h"

// some macro to shorten template notations
#define templated template<typename numberType, typename errorType, typename preciseType>
#define Snum S<numberType,errorType,preciseType>
#define INTEGER_CAST_CONSTRUCTOR(n) S((numberType)n, (preciseType)n - (numberType)n)

//-------------------------------------------------------------------------------------------------
// SHAMAN OPERATIONS

/*
 * the base SHAMAN class, represents a number and its error
 */
templated class S
{
public :
    // trueNumber = number + error
    numberType number; // current computed number
    errorType error; // current error
    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
        bool isNumericalZero;
    #endif
    #ifdef DOUBT_LEVEL_FIELD_ENABLED
        int doubtLevel;
    #endif

    // constructors
    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
        #ifdef DOUBT_LEVEL_FIELD_ENABLED
            inline S(numberType numberArg, errorType errorArg, bool isNumericalZeroArg, int doubt): number(numberArg), error(errorArg), isNumericalZero(isNumericalZeroArg), doubtLevel(doubt) {};
            inline S(numberType numberArg, errorType errorArg, bool isNumericalZeroArg): S(numberArg, errorArg, isNumericalZeroArg, 0) {};
        #else
            inline S(numberType numberArg, errorType errorArg, bool isNumericalZeroArg): number(numberArg), error(errorArg), isNumericalZero(isNumericalZeroArg) {};
        #endif
        inline S(numberType numberArg, errorType errorArg): S(numberArg, errorArg, non_significativ(numberArg,errorArg)) {};
        inline S(numberType numberArg): S(numberArg,0,false) {}; // we accept implicit cast from T to S<T>
    #else
        inline S(numberType numberArg, errorType errorArg): number(numberArg), error(errorArg) {};
        inline S(numberType numberArg): S(numberArg,0) {}; // we accept implicit cast from T to S<T>
    #endif
    inline S(short int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned short int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned long int n):INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(long long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned long long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
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
    template<typename n, typename e, typename p>
    inline explicit S(const S<n,e,p>& s): S(s.number, s.error) {}; // requires explicit cast from other S types
    template<typename T> S(T s) = delete; // refuse cast from types other than numberType
    #else
    template<typename n, typename e, typename p>
    inline S(const S<n,e,p>& s): S(s.number, s.error) {}; // allow cast from any Stype to any Stype
    #endif

    // arithmetic operators
    S& operator++(int);
    S& operator--(int);
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
templated bool isfinite(const Snum& n);
templated bool isnan(const Snum& n);
templated const Snum abs(const Snum& n);
templated const Snum fabs(const Snum& n);
templated const Snum floor(const Snum& n);
templated const Snum sqrt(const Snum& n);
templated const Snum cbrt(const Snum& n);
templated const Snum exp(const Snum& n);
templated const Snum frexp(const Snum& n, int* exp);
templated const Snum ldexp(const Snum& n, int exp);
templated const Snum erf(const Snum& n);
templated const Snum sin(const Snum& n);
templated const Snum cos(const Snum& n);
templated const Snum acos(const Snum& n);
templated const Snum tan(const Snum& n);
templated const Snum pow(const Snum& n1, const Snum& n2);
templated const Snum min(const Snum& n1, const Snum& n2);
templated const Snum max(const Snum& n1, const Snum& n2);
templated const Snum fma(const Snum& n1, const Snum& n2, const Snum& n3);

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
// OPENMP

#ifdef _OPENMP
// require openMP 4.0+ to get reductions on user defined types
#pragma omp declare reduction(+:Sfloat : omp_out=omp_in+omp_out)    initializer(omp_priv=Sfloat(0.f))
#pragma omp declare reduction(+:Sdouble: omp_out=omp_in+omp_out)	initializer(omp_priv=Sdouble(0.))
#pragma omp declare reduction(-:Sfloat : omp_out=omp_in+omp_out)	initializer(omp_priv=Sfloat(0.f))
#pragma omp declare reduction(-:Sdouble: omp_out=omp_in+omp_out)	initializer(omp_priv=Sdouble(0.))
#pragma omp declare reduction(*:Sfloat : omp_out=omp_in*omp_out)	initializer(omp_priv=Sfloat(1.f))
#pragma omp declare reduction(*:Sdouble: omp_out=omp_in*omp_out)	initializer(omp_priv=Sdouble(1.))
#endif //_OPENMP

//-------------------------------------------------------------------------------------------------
// SOURCE

#include "src/Methods.h"
#include "src/Operators.h"
#include "src/Functions.h"

//-------------------------------------------------------------------------------------------------

#undef templated
#undef Snum

#endif //SHAMAN_H