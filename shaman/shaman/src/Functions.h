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
    if (n >= 0)
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
    preciseType preciseCorrectedResult = std::floor((preciseType) n.number + n.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

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
        numberType remainder = - std::fma(result, result, - n.number);
        newError = (remainder + n.error) / (result + result);
    }
    // alternativ formula that is not based on derivation :
    // newError = (errorType) std::sqrt((preciseType) std::abs(n.number + (n.error - remainder))) - result;

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// cubic root
// TODO might be improved using a formula similar to the one used for the square root
templated inline const Snum cbrt(const Snum& n)
{
    numberType result = std::cbrt(n.number);
    preciseType preciseCorrectedResult = std::cbrt((preciseType) n.number + n.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// exp
templated inline const Snum exp(const Snum& n)
{
    numberType result = std::exp(n.number);
    preciseType preciseCorrectedResult = std::exp((preciseType) n.number + n.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// frexp
templated inline const Snum frexp(const Snum& n, int* exp)
{
    numberType result = std::frexp(n.number, exp);
    int dummyExp;
    preciseType preciseCorrectedResult = std::frexp((preciseType) n.number + n.error, &dummyExp);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// ldexp
templated inline const Snum ldexp(const Snum& n, int exp)
{
    numberType result = std::ldexp(n.number, exp);
    preciseType preciseCorrectedResult = std::ldexp((preciseType) n.number + n.error, exp);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// erf
templated inline const Snum erf(const Snum& n)
{
    numberType result = std::erf(n.number);
    preciseType preciseCorrectedResult = std::erf((preciseType) n.number + n.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// sin
templated inline const Snum sin(const Snum& n)
{
    numberType result = std::sin(n.number);
    preciseType preciseCorrectedResult = std::sin((preciseType) n.number + n.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// cos
templated inline const Snum cos(const Snum& n)
{
    numberType result = std::cos(n.number);
    preciseType preciseCorrectedResult = std::cos((preciseType) n.number + n.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// acos
templated inline const Snum acos(const Snum& n)
{
    numberType result = std::acos(n.number);
    preciseType correctedNumber = n.number + n.error;
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
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// tan
templated inline const Snum tan(const Snum& n)
{
    numberType result = std::tan(n.number);
    preciseType preciseCorrectedResult = std::tan((preciseType) n.number + n.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
    if (n.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};

// pow
templated inline const Snum pow(const Snum& n1, const Snum& n2)
{
    numberType result = std::pow(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::pow((preciseType) n1.number + n1.error, (preciseType) n2.number + n2.error);
    errorType newError = (errorType) (preciseCorrectedResult - result);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
    if (n1.non_significativ() || n2.non_significativ())
    {
        #ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstablePowerFunctions++;
        NumericalDebugger::unstability();
        #endif
        doubtLevel++;
    }
#endif

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n1.non_significativ() || n2.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Sfunction2_casts(pow);

// min
templated inline const Snum min(const Snum& n1, const Snum& n2)
{
    UNSTABLE_BRANCH_TEST;

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
    UNSTABLE_BRANCH_TEST;

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

    numberType remainder = EFT::eftErrFma(n1.number, n2.number, n3.number, result);
    //errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error) + n3.error;
    errorType newError = std::fma(n2.number, n1.error, std::fma(n1.number, n2.error, remainder + n3.error));

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel, n3.doubtLevel);
#endif

    CANCELATION_TEST(Snum::minPrecision(n1,Snum::minPrecision(n2,n3)));

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n1.non_significativ() || n2.non_significativ() || n3.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Sfunction3_casts(fma);

#endif //SHAMAN_FUNCTIONS_H
