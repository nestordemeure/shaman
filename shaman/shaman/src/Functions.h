//
// Created by demeuren on 02/05/18.
//

#ifndef SHAMAN_FUNCTIONS_H
#define SHAMAN_FUNCTIONS_H

#include "../Shaman.h"
#include "Eft.h"

//-----------------------------------------------------------------------------
// CASTING MACROS

/*
 * this section uses templates and macro to take a function and produce overloads for all the needed combination of types
 * while preserving implicit cast conventions
 */

#ifdef EXPLICIT_CASTING
// function (between n and S<n,e,p> only)
#define set_Sfunction2_casts(FUN) \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const n& n1, const S<n,e,p>& n2) \
{ \
    return FUN(S<n,e,p>(n1), n2); \
} \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const S<n,e,p>& n1, const n& n2) \
{ \
    return FUN(n1, S<n,e,p>(n2)); \
} \

// function with 3 arguments (between n and S<n,e,p> only)
#define set_Sfunction3_casts(FUN) \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const n& n1, const n& n2, const S<n,e,p>& n3) \
{ \
    return FUN(S<n,e,p>(n1), S<n,e,p>(n2), n3); \
} \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const n& n1, const S<n,e,p>& n2, const n& n3) \
{ \
    return FUN(S<n,e,p>(n1), n2, S<n,e,p>(n3)); \
} \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const S<n,e,p>& n1, const n& n2, const n& n3) \
{ \
    return FUN(n1, S<n,e,p>(n2), S<n,e,p>(n3)); \
} \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const n& n1, const S<n,e,p>& n2, const S<n,e,p>& n3) \
{ \
    return FUN(S<n,e,p>(n1), n2, n3); \
} \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const S<n,e,p>& n1, const S<n,e,p>& n2, const n& n3) \
{ \
    return FUN(n1, n2, S<n,e,p>(n3)); \
} \
template<typename n, typename e, typename p> \
inline const S<n,e,p> FUN (const S<n,e,p>& n1, const n& n2, const S<n,e,p>& n3) \
{ \
    return FUN(n1, S<n,e,p>(n2), n3); \
} \

#else

// takes three values and builds a S type around the type C++ would use as a return type for their fma
#define SreturnType3(t1,t2,t3) decltype(makeStype(std::fma(t1, t2, t3)))

// defines overload for function taking two arguments
#define set_Sfunction2_casts(FUN) \
template<typename N, typename E, typename P, typename arithmeticTYPE(T)> \
inline auto FUN (const S<N,E,P>& n1, const T& n2) -> SreturnType(n1.number,n2)\
{ \
    return FUN(SreturnType(n1.number,n2)(n1), SreturnType(n1.number,n2)(n2)); \
} \
template<typename N, typename E, typename P, typename arithmeticTYPE(T)> \
inline auto FUN (const T& n1, const S<N,E,P>& n2) -> SreturnType(n1, n2.number)\
{ \
    return FUN(SreturnType(n1,n2.number)(n1), SreturnType(n1,n2.number)(n2)); \
} \
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2> \
inline auto FUN (const S<N1,E1,P1>& n1, const S<N2,E2,P2>& n2) -> SreturnType(n1.number, n2.number) \
{ \
    return FUN(SreturnType(n1.number,n2.number)(n1), SreturnType(n1.number,n2.number)(n2)); \
} \

// defines overload for function taking three arguments
#define set_Sfunction3_casts(FUN) \
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2, typename N3, typename E3, typename P3> \
inline auto FUN (const S<N1,E1,P1>& n1, const S<N2,E2,P2>& n2, const S<N3,E3,P3>& n3) -> SreturnType3(n1.number,n2.number,n3.number) \
{ \
    return FUN(SreturnType3(n1.number,n2.number,n3.number)(n1), SreturnType3(n1.number,n2.number,n3.number)(n2), SreturnType3(n1.number,n2.number,n3.number)(n3)); \
} \
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2, typename arithmeticTYPE(T)> \
inline auto FUN (const T& n1, const S<N1,E1,P1>& n2, const S<N2,E2,P2>& n3) -> SreturnType3(n1,n2.number,n3.number) \
{ \
    return FUN(SreturnType3(n1,n2.number,n3.number)(n1), SreturnType3(n1,n2.number,n3.number)(n2), SreturnType3(n1,n2.number,n3.number)(n3)); \
} \
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2, typename arithmeticTYPE(T)> \
inline auto FUN (const S<N1,E1,P1>& n1, const T& n2, const S<N2,E2,P2>& n3) -> SreturnType3(n1.number,n2,n3.number)\
{ \
    return FUN(SreturnType3(n1.number,n2,n3.number)(n1), SreturnType3(n1.number,n2,n3.number)(n2), SreturnType3(n1.number,n2,n3.number)(n3)); \
} \
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2, typename arithmeticTYPE(T)> \
inline auto FUN (const S<N1,E1,P1>& n1, const S<N2,E2,P2>& n2, const T& n3) -> SreturnType3(n1.number,n2.number,n3)\
{ \
    return FUN(SreturnType3(n1.number,n2.number,n3)(n1), SreturnType3(n1.number,n2.number,n3)(n2), SreturnType3(n1.number,n2.number,n3)(n3)); \
} \
template<typename N, typename E, typename P, typename arithmeticTYPE(T1), typename arithmeticTYPE(T2)> \
inline auto FUN (const S<N,E,P>& n1, const T1& n2, const T2& n3) -> SreturnType3(n1.number,n2,n3)\
{ \
    return FUN(SreturnType3(n1.number,n2,n3)(n1), SreturnType3(n1.number,n2,n3)(n2), SreturnType3(n1.number,n2,n3)(n3)); \
} \
template<typename N, typename E, typename P, typename arithmeticTYPE(T1), typename arithmeticTYPE(T2)> \
inline auto FUN (const T1& n1, const S<N,E,P>& n2, const T2& n3) -> SreturnType3(n1,n2.number,n3) \
{ \
    return FUN(SreturnType3(n1,n2.number,n3)(n1), SreturnType3(n1,n2.number,n3)(n2), SreturnType3(n1,n2.number,n3)(n3)); \
} \
template<typename N, typename E, typename P, typename arithmeticTYPE(T1), typename arithmeticTYPE(T2)> \
inline auto FUN (const T1& n1, const T2& n2, const S<N,E,P>& n3) -> SreturnType3(n1,n2,n3.number)\
{ \
    return FUN(SreturnType3(n1,n2,n3.number)(n1), SreturnType3(n1,n2,n3.number)(n2), SreturnType3(n1,n2,n3.number)(n3)); \
} \

#endif
//-----------------------------------------------------------------------------
// MATHEMATICAL FUNCTIONS

// isifnite
templated inline bool isfinite(const Snum& n)
{
    return std::isfinite(n.number);
};

// isnan
templated inline bool isnan(const Snum& n)
{
    return std::isnan(n.number);
};

// abs
templated inline const Snum abs(const Snum& n)
{
    if (n >= 0.)
    {
        return n;
    }
    else
    {
        return -n;
    }
};

// fabs
templated inline const Snum fabs(const Snum& n)
{
    return abs(n);
};

// floor
templated inline const Snum floor(const Snum& n)
{
    numberType result = std::floor(n.number);
    preciseType preciseCorrectedResult = std::floor(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// ceil
templated inline const Snum ceil(const Snum& n)
{
    numberType result = std::ceil(n.number);
    preciseType preciseCorrectedResult = std::ceil(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// sqrt
templated inline const Snum sqrt(const Snum& n)
{
    numberType result = std::sqrt(n.number);

    errorType newError;
    if (result == 0)
    {
        newError = (errorType) std::sqrt((preciseType) std::abs(n.error));
    }
    else
    {
        numberType remainder = EFT::RemainderSqrt(n.number, result);
        newError = (remainder + n.error) / (result + result);
    }
    // alternativ formula that is not based on derivation :
    // newError = (errorType) std::sqrt((preciseType) std::abs(n.number + (n.error - remainder))) - result;

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// cubic root
// TODO might be improved using a formula similar to the one used for the square root
templated inline const Snum cbrt(const Snum& n)
{
    numberType result = std::cbrt(n.number);
    preciseType preciseCorrectedResult = std::cbrt(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// exp
templated inline const Snum exp(const Snum& n)
{
    numberType result = std::exp(n.number);
    preciseType preciseCorrectedResult = std::exp(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// exp2
templated inline const Snum exp2(const Snum& n)
{
    numberType result = std::exp2(n.number);
    preciseType preciseCorrectedResult = std::exp2(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// frexp
templated inline const Snum frexp(const Snum& n, int* exp)
{
    numberType result = std::frexp(n.number, exp);
    int dummyExp;
    preciseType preciseCorrectedResult = std::frexp(n.corrected_number(), &dummyExp);
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// ldexp
templated inline const Snum ldexp(const Snum& n, int exp)
{
    numberType result = std::ldexp(n.number, exp);
    preciseType preciseCorrectedResult = std::ldexp(n.corrected_number(), exp);
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// erf
templated inline const Snum erf(const Snum& n)
{
    numberType result = std::erf(n.number);
    preciseType preciseCorrectedResult = std::erf(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// log
templated inline const Snum log(const Snum& n)
{
    numberType result = std::log(n.number);
    preciseType correctedNumber = n.corrected_number();

    preciseType preciseCorrectedResult;
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log(correctedNumber);
    }

    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// log2
templated inline const Snum log2(const Snum& n)
{
    numberType result = std::log2(n.number);
    preciseType correctedNumber = n.corrected_number();

    preciseType preciseCorrectedResult;
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log2(correctedNumber);
    }

    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// log10
templated inline const Snum log10(const Snum& n)
{
    numberType result = std::log10(n.number);
    preciseType correctedNumber = n.corrected_number();

    preciseType preciseCorrectedResult;
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log10(correctedNumber);
    }

    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// sin
templated inline const Snum sin(const Snum& n)
{
    numberType result = std::sin(n.number);
    preciseType preciseCorrectedResult = std::sin(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// sinh
templated inline const Snum sinh(const Snum& n)
{
    numberType result = std::sinh(n.number);
    preciseType preciseCorrectedResult = std::sinh(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// asinh
templated inline const Snum asinh(const Snum& n)
{
    numberType result = std::asinh(n.number);
    preciseType preciseCorrectedResult = std::asinh(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// cos
templated inline const Snum cos(const Snum& n)
{
    numberType result = std::cos(n.number);
    preciseType preciseCorrectedResult = std::cos(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// cosh
templated inline const Snum cosh(const Snum& n)
{
    numberType result = std::cosh(n.number);
    preciseType preciseCorrectedResult = std::cosh(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// acosh
templated inline const Snum acosh(const Snum& n)
{
    numberType result = std::acosh(n.number);
    preciseType correctedNumber = n.corrected_number();

    preciseType preciseCorrectedResult;
    if (correctedNumber < 1.)
    {
        preciseCorrectedResult = 0.;
    }
    else
    {
        preciseCorrectedResult = std::acosh(correctedNumber);
    }

    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// acos
templated inline const Snum acos(const Snum& n)
{
    numberType result = std::acos(n.number);
    preciseType correctedNumber = n.corrected_number();

    preciseType preciseCorrectedResult;
    if (correctedNumber > 1)
    {
        preciseCorrectedResult = 0;
    }
    else if (correctedNumber < -1)
    {
        preciseCorrectedResult = M_PI;
    }
    else
    {
        preciseCorrectedResult = std::acos(correctedNumber);
    }

    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// asin
templated inline const Snum asin(const Snum& n)
{
    numberType result = std::asin(n.number);
    preciseType correctedNumber = n.corrected_number();

    preciseType preciseCorrectedResult;
    if (correctedNumber > 1)
    {
        preciseCorrectedResult = M_PI/2.;
    }
    else if (correctedNumber < -1)
    {
        preciseCorrectedResult = -M_PI/2;
    }
    else
    {
        preciseCorrectedResult = std::asin(correctedNumber);
    }

    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// atan
templated inline const Snum atan(const Snum& n)
{
    numberType result = std::atan(n.number);
    preciseType preciseCorrectedResult = std::atan(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// tan
templated inline const Snum tan(const Snum& n)
{
    numberType result = std::tan(n.number);
    preciseType preciseCorrectedResult = std::tan(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// tanh
templated inline const Snum tanh(const Snum& n)
{
    numberType result = std::tanh(n.number);
    preciseType preciseCorrectedResult = std::tanh(n.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// atanh
templated inline const Snum atanh(const Snum& n)
{
    numberType result = std::atanh(n.number);
    preciseType correctedNumber = n.corrected_number();

    preciseType preciseCorrectedResult;
    if(correctedNumber > 1. || correctedNumber < 1.)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::atanh(correctedNumber);
    }

    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// atan2
templated inline const Snum atan2(const Snum& n1, const Snum& n2)
{
    numberType result = std::atan2(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::atan2(n1.corrected_number(), n2.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
    if (n1.non_significant() || n2.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstablePowerFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n1.non_significant() || n2.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Sfunction2_casts(atan2);

// pow
templated inline const Snum pow(const Snum& n1, const Snum& n2)
{
    numberType result = std::pow(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::pow(n1.corrected_number(), n2.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
    if (n1.non_significant() || n2.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstablePowerFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n1.non_significant() || n2.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Sfunction2_casts(pow);

// min
templated inline const Snum min(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    if (n1.number <= n2.number)
    {
        return n1;
    }
    else
    {
        return n2;
    }
};
set_Sfunction2_casts(min);

// max
templated inline const Snum max(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    if (n1.number >= n2.number)
    {
        return n1;
    }
    else
    {
        return n2;
    }
};
set_Sfunction2_casts(max);

// fma
templated inline const Snum fma(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::fma(n1.number, n2.number, n3.number);

    numberType remainder = EFT::ErrorFma(n1.number, n2.number, n3.number, result);
    //errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error) + n3.error;
    errorType newError = std::fma(n2.number, n1.error, std::fma(n1.number, n2.error, remainder + n3.error));

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel, n3.doubtLevel);
    #endif

    #ifdef CANCELATION_DEBUGGER
    cancelationTest(Snum::minPrecision(n1,Snum::minPrecision(n2,n3)), result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n1.non_significant() || n2.non_significant() || n3.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Sfunction3_casts(fma);

// hypot
templated inline const Snum hypot(const Snum& n1, const Snum& n2)
{
    numberType result = std::hypot(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
    if (n1.non_significant() || n2.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstablePowerFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n1.non_significant() || n2.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Sfunction2_casts(hypot);

// hypot
templated inline const Snum hypot(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::hypot(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number(), n3.corrected_number());
    errorType newError = (errorType) (preciseCorrectedResult - result);

    #ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel =  std::max( std::max(n1.doubtLevel, n2.doubtLevel), n3.doubtLevel);
    if (n1.non_significant() || n2.non_significant() || n3.non_significant())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstablePowerFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
    #endif

    #ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significant(result, newError);
    #endif

    #ifdef NUMERICAL_ZERO_DEBUGGER
    numericalZeroTest(isNumericalZero, n1.non_significant() || n2.non_significant() || n3.non_significant());
    #endif

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Sfunction3_casts(hypot);

//-----------------------------------------------------------------------------

#undef set_Sfunction2_casts
#undef set_Sfunction3_casts

#endif //SHAMAN_FUNCTIONS_H