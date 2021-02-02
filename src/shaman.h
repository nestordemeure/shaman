#ifndef SHAMAN_H
#define SHAMAN_H

// to get M_PI on windows compiler
#define _USE_MATH_DEFINES

#include <string>
#include <limits>
#include <atomic>
#include <memory>

#ifdef SHAMAN_TAGGED_ERROR
#include <shaman/tagged/error_sum.h>
#else
#define FUNCTION_BLOCK
#define LOCAL_BLOCK(text)
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
    inline S(numberType numberArg, errorType errorArg, error_sum<errorType> errorCompArg): number(numberArg), error(errorArg), errorComposants(errorCompArg) {};
    inline S(numberType numberArg): number(numberArg), error(0.), errorComposants() {}; // we accept implicit cast from T to S<T>
    inline S(): number(0.), error(0.), errorComposants() {};
    // casting
    #define INTEGER_CAST_CONSTRUCTOR(n) number((numberType)n), error((preciseType)n - (numberType)n), errorComposants(ShamanGlobals::tagIntegerCast, (preciseType)n - (numberType)n)
    template<typename n, typename e, typename p> inline S(const S<n,e,p>& s): number(s.number), error(s.error), errorComposants(s.errorComposants) {};
    template<typename n, typename e, typename p> inline S(volatile S<n,e,p>& s): number(s.number), error(s.error), errorComposants(const_cast<error_sum<e>&>(s.errorComposants)) {};
    #else
    // base constructors
    inline S(numberType numberArg, errorType errorArg): number(numberArg), error(errorArg) {};
    inline S(numberType numberArg): number(numberArg), error(0.) {}; // we accept implicit cast from T to S<T>
    inline S(): number(0.), error(0.) {};
    // casting
    #define INTEGER_CAST_CONSTRUCTOR(n) number((numberType)n), error((preciseType)n - (numberType)n)
    template<typename n, typename e, typename p> inline S(const S<n,e,p>& s): number(s.number), error(s.error) {};
    template<typename n, typename e, typename p> inline S(volatile S<n,e,p>& s): number(s.number), error(s.error) {};
    #endif

    // casting
    inline operator bool() const { return (bool) number; };
    inline operator short int() const { return (short int) number; };
    inline operator unsigned short int() const { return (unsigned short int) number; };
    inline operator int() const { return (int) number; };
    inline operator unsigned int() const { return (unsigned int) number; };
    inline operator long int() const { return (long int) number; };
    inline operator unsigned long int() const { return (unsigned long int) number; };
    inline operator long long int() const { return (long long int) number; };
    inline operator unsigned long long int() const { return (unsigned long long int) number; };
    inline explicit operator float() const { return (float) number; };
    inline explicit operator double() const { return (double) number; };
    inline explicit operator long double() const { return (long double) number; };
    inline S(short int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned short int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned long int n):INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(long long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    inline S(unsigned long long int n): INTEGER_CAST_CONSTRUCTOR(n) {};
    #undef INTEGER_CAST_CONSTRUCTOR

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

// std + shaman
#ifdef NO_SHAMAN
namespace Sstd = std;
#else
namespace Sstd
{
    using namespace std;
    using namespace Shaman;
    // mathematical functions
    templated const Snum abs(const Snum &n);
    templated const Snum fabs(const Snum &n);
    templated const Snum sqrt(const Snum &n);
    templated const Snum cbrt(const Snum &n);
    templated const Snum pow(const Snum &n1, const Snum &n2);
    templated const Snum exp(const Snum &n);
    templated const Snum exp2(const Snum &n);
    templated const Snum frexp(const Snum &n, int *exp);
    templated const Snum ldexp(const Snum &n, int exp);
    templated const Snum log(const Snum &n);
    templated const Snum log2(const Snum &n);
    templated const Snum log10(const Snum &n);
    templated const Snum logb(const Snum &n);
    templated const Snum sin(const Snum &n);
    templated const Snum cos(const Snum &n);
    templated const Snum tan(const Snum &n);
    templated const Snum asin(const Snum &n);
    templated const Snum acos(const Snum &n);
    templated const Snum atan(const Snum &n);
    templated const Snum atan2(const Snum &n1, const Snum &n2);
    templated const Snum sinh(const Snum &n);
    templated const Snum cosh(const Snum &n);
    templated const Snum tanh(const Snum &n);
    templated const Snum asinh(const Snum &n);
    templated const Snum acosh(const Snum &n);
    templated const Snum atanh(const Snum &n);
    templated const Snum erf(const Snum &n);
    templated const Snum erff(const Snum &n);
    templated const Snum erfl(const Snum &n);
    templated const Snum floor(const Snum &n);
    templated const Snum ceil(const Snum &n);
    templated const Snum trunc(const Snum &n);
    templated const Snum scalbn(const Snum &n, int power);
    templated const Snum min(const Snum &n1, const Snum &n2);
    templated const Snum max(const Snum &n1, const Snum &n2);
    templated const Snum fmin(const Snum &n1, const Snum &n2);
    templated const Snum fmax(const Snum &n1, const Snum &n2);
    templated const Snum copysign(const Snum &n1, const Snum &n2);
    templated const Snum hypot(const Snum &n1, const Snum &n2);
    templated const Snum hypot(const Snum &n1, const Snum &n2, const Snum &n3);
    templated const Snum fma(const Snum &n1, const Snum &n2, const Snum &n3);
    templated bool isfinite(const Snum &n);
    templated bool isinf(const Snum &n);
    templated bool isnan(const Snum &n);
    templated bool signbit(const Snum &n);
	templated const Snum expm1(const Snum& n);
	templated const Snum ilogb(const Snum& n);
	templated const Snum modf(const Snum& n, Snum* intpart);
	templated const Snum log1p(const Snum& n);
	templated const Snum scalbln(const Snum &n, long int power);
	templated const Snum erfc(const Snum& n);
    templated const Snum erfcf(const Snum& n);
    templated const Snum erfcl(const Snum& n);
	templated const Snum tgamma(const Snum& n);
	templated const Snum lgamma(const Snum& n);
	templated const Snum round(const Snum& n);
	templated const Snum rint(const Snum& n);
	templated const Snum nearbyint(const Snum& n);
	templated const Snum fmod(const Snum& n1, const Snum& n2);
	templated const long int lround(const Snum& n);
	templated const long long int llround(const Snum& n);
	templated const long int lrint(const Snum& n);
	templated const long long int llrint(const Snum& n);
	templated const Snum remainder(const Snum& n1, const Snum& n2);
	templated const Snum remquo(const Snum& n1, const Snum& n2, int* quot);
	templated const Snum nan(const char* tagp);
	templated const Snum nextafter(const Snum& n1, const Snum& n2);
	templated const Snum nexttoward(const Snum& n1, const Snum& n2);
	templated const Snum fdim(const Snum& n1, const Snum& n2);
	templated const int fpclassify(const Snum& x);
	templated bool isnormal(const Snum& n);
	templated const bool isgreater(const Snum& n1, const Snum& n2);
	templated const bool isgreaterequal(const Snum& n1, const Snum& n2);
	templated const bool isless(const Snum& n1, const Snum& n2);
	templated const bool islessequal(const Snum& n1, const Snum& n2);
	templated const bool islessgreater(const Snum& n1, const Snum& n2);
	templated const bool isunordered(const Snum& n1, const Snum& n2);
    //methods
    templated std::string to_string(const Snum &n);
    template<typename T> const char* to_Cstring(const T &n);
}
#endif

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
#include <shaman/helpers/shaman_openmp.h>

//-------------------------------------------------------------------------------------------------

#undef templated
#undef Snum
#endif //SHAMAN_H
