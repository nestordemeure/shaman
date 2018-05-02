//
// Created by demeuren on 02/05/18.
//

#ifndef SHAMAN_SIGNIFICATIVITY_H
#define SHAMAN_SIGNIFICATIVITY_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "../Shaman.h"

//-----------------------------------------------------------------------------
// SIGNIFICATIV DIGITS

/*
 * returns the number of (relative) significative digits of a couple (number,error)
 *
 * the exact 0 is handled by a special case (otherwise it cannot have any significativ digits)
 */
templated inline numberType Snum::digits(numberType number, errorType error)
{
    if (error == 0)
    {
        return INFINITY;
    }
    else if (number == 0)
    {
        return std::max(0., -log10(std::abs(error)) - 1);
    }
    else
    {
        numberType relativeError = std::abs((numberType) error / number);

        if (relativeError >= 1)
        {
            return 0;
        }
        else
        {
            return -std::log10(relativeError);
        }
    }
}

/*
 * returns the number of significative digits of a S
 */
templated inline numberType Snum::digits(const Snum &n)
{
    return digits(n.number, n.error);
}

//-----------------------------------------------------------------------------
// UNSTABILITY DETECTION

/*
 * returns true if the couple (number,error) has no significative digits in the base
 *
 * NOTE : 'error != 0' is facultative
 * it slightly improves the performances on some test cases
 * since it is common to have numbers with 0 error
 * (any number that just been turned into a S)
 */
templated inline bool Snum::non_significativ(numberType number, errorType error)
{
    int base = 10;
    return (error != 0) && (std::abs(number) < base * std::abs(error));
}

/*
 * returns true if the current S has no significative digits in the base
 */
templated inline bool Snum::non_significativ() const
{
#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    return isNumericalZero;
#else
    return non_significativ(number, error);
#endif
}

/*
 * return the Snum with the lowest precision
 */
templated inline Snum Snum::minPrecision(const Snum &n1, const Snum &n2)
{
    if (std::abs(n1.error * n2.number) > std::abs(n1.number * n2.error))
    {
        return n1;
    }
    else
    {
        return n2;
    }
}

/*
 * detects cancelations
 */
templated inline bool Snum::isCancelation(const Snum &n, numberType result, errorType resultingError)
{
    int cancel_level = 4;
    int base = 10;

    // have we lost more than cancel_level significative digits ?
    return (n.error != 0) &&
           (std::abs(resultingError * n.number) > pow(base, cancel_level) * std::abs(n.error * result));
}

/*
 * detects unstable branchings
 */
templated inline bool Snum::isUnstableBranchings(const Snum &n1, const Snum &n2)
{
    return non_significativ(n1.number - n2.number, n1.error - n2.error);
}

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
        os << " (#unstabilities=" << n.doubtLevel << ')';
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
// CASTING MACROS

#ifndef EXPLICIT_CASTING

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

#endif

//-----------------------------------------------------------------------------
#endif //SHAMAN_SIGNIFICATIVITY_H