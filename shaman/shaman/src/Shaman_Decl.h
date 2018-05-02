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




#endif //SHAMAN_DECL_H
