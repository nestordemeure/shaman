//
// Created by nestor on 17/01/18.
//

#ifndef SHAMAN_DECL_H
#define SHAMAN_DECL_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include "../Shaman.h"
#include "NumericalDebugger.h"
#include "Eft.h"

//-----------------------------------------------------------------------------
// OPERATOR CASTING MACROS
#ifndef EXPLICIT_CASTING

/*
 * this section uses templates and macro (and the curry howard isomorphism)
 * to take an operator and produce overloads for all the needed combinaison of types
 * while preserving C++ implicit cast conventions
 */

#define arithmeticTYPE(T) T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type

// takes a value and builds an S type around its type
template<typename T>
inline S<T,T,T> makeStype(T t) { return S<T,T,T>(t); }; // default case
inline Sfloat makeStype(float t) { return Sfloat(t); };
inline Sdouble makeStype(double t) { return Sdouble(t); };
inline Slong_double makeStype(long double t) { return Slong_double(t); };
templated inline Snum makeStype(Snum s) { return s; };

// takes two values and builds a S type around the type C++ would use as a return type for their sum
#define SreturnType(t1,t2) decltype(makeStype(t1 + t2))
// takes three values and builds a S type around the type C++ would use as a return type for their fma
#define SreturnType3(t1,t2,t3) decltype(makeStype(std::fma(t1, t2, t3)))

#endif

//-----------------------------------------------------------------------------
// DEBUGGING MACROS

/*
 * TODO : some of those macro could be replaced by inline functions
 * pro : improved readability, reduced probability of errors
 * con : gdb will need to travel through more functions
 */

// macro used in constructors
#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    #define ISNUMERICALZERO , isNumericalZero
#else
    #define ISNUMERICALZERO
#endif

// macro used in constructors
#ifdef DOUBT_LEVEL_FIELD_ENABLED
    #define DOUBTLEVEL , doubtLevel
#else
    #define DOUBTLEVEL
#endif

// macro that encapsulate cancellation test
#ifdef CANCELATION_DEBUGGER
    #define CANCELATION_TEST(n_minPrecision) \
        if (Snum::isCancelation(n_minPrecision, result, newError)) \
        { \
        NumericalDebugger::cancelations++; \
        NumericalDebugger::unstability(); \
        }
#else
    #define CANCELATION_TEST(n_minPrecision)
#endif

// macro that encapsulate numerical zero test
#ifdef NUMERICAL_ZERO_DEBUGGER
    #define NUMERICAL_ZERO_TEST(is_non_significativ) \
        if (isNumericalZero && !(is_non_significativ)) \
        { \
        NumericalDebugger::numericalZeros++; \
        NumericalDebugger::unstability(); \
        }
#else
    #define NUMERICAL_ZERO_TEST(is_non_significativ)
#endif

// macro that encapsulate branch unstability test
#ifdef UNSTABLE_BRANCH_DEBUGGER
    #define UNSTABLE_BRANCH_TEST \
        if (Snum::isUnstableBranchings(n1, n2)) \
        { \
        NumericalDebugger::unstableBranchings++; \
        NumericalDebugger::unstability(); \
        }
#else
    #define UNSTABLE_BRANCH_TEST
#endif

//-----------------------------------------------------------------------------
// STRING CONVERSIONS

/*
 * streaming operator, displays only the significative digits
 * (alternative version : outputs the number, the error and the number of significative digits)
 */
templated inline std::ostream& operator<<(std::ostream& os, const Snum& n)
{
    // full information
    //os << n.number << " (error:" << n.error << " digits:" << Snum::digits(n) << ')';
    int nbDigitsMax = 17;
    numberType fdigits = std::floor(Snum::digits(n));

    if (std::isnan(fdigits))
    {
        os << "@nan";
    }
    else if (fdigits <= 0)
    {
        // the first zeros might be valid
        int digits = std::floor(Snum::digits(0, n.error));

        if ((std::abs(n.number) >= 1) || (digits <= 0))
        {
            // the number has no meaning
            os << "@.0";
        }
        else
        {
            // some zero are significatives
            digits = std::min(nbDigitsMax, digits);
            os << std::scientific << std::setprecision(digits-1) << 0.0; // TODO should we add a '@' prefix ?
        }
    }
    else
    {
        int digits = std::min((numberType) nbDigitsMax, fdigits);
        os << std::scientific << std::setprecision(digits-1) << n.number;
    }

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    if (n.doubtLevel > 0)
    {
        os << " (doubt=" << n.doubtLevel << ')';
    }
    #endif

    return os;
}

/*
 * convert a Ntype into a string
 *
 * using the streaming operator
 * there is probably a more efficient implementation
 * but it is the easier way to benefit from std::scientific and std::setprecision
 */
templated Snum::operator std::string() const
{
    std::ostringstream stream;
    stream << this;
    return stream.str();
}

/*
 * extraction operator to cast a stream into an Snum
 */
templated std::istream& operator>>(std::istream& is, Snum& n)
{
    // extracts the number
    numberType num;
    is >> num;

    // modifies the Snum
    n.number = num;
    n.error = 0;

    return is;
}



#endif //SHAMAN_DECL_H
