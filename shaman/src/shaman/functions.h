#pragma once

#include "eft.h"

//-----------------------------------------------------------------------------
// CASTING MACROS

/*
 * this section uses templates and macro to take a function and produce overloads for all the needed combination of types
 * while preserving implicit cast conventions
 */

// takes three values and builds a S type around the type C++ would use as a return type for their fma
#define SreturnType3(t1,t2,t3) decltype(makeStype(std::fma(t1, t2, t3)))

// defines overload for function taking two arguments
#define set_Sfunction2_casts(FUN) \
namespace Sstd \
{ \
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
}\
using Sstd::FUN; \

// defines overload for function taking three arguments
#define set_Sfunction3_casts(FUN) \
namespace Sstd \
{ \
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
} \
using Sstd::FUN; \

//-----------------------------------------------------------------------------
// TEST BASED FUNCTIONS

// isfinite
templated inline bool Sstd::isfinite(const Snum& n)
{
    return std::isfinite(n.number);
};
using Sstd::isfinite;

// isinf
templated inline bool Sstd::isinf(const Snum& n)
{
    return std::isinf(n.number);
};
using Sstd::isinf;

// isnan
templated inline bool Sstd::isnan(const Snum& n)
{
    return std::isnan(n.number);
};
using Sstd::isnan;

// signbit
templated inline bool Sstd::signbit(const Snum& n)
{
    Snum::checkUnstableBranch(n, Snum(typename Snum::NumberType(0.)));
    return std::signbit(n.number);
};
using Sstd::signbit;

// abs
templated inline const Snum Sstd::abs(const Snum& n)
{
    Snum::checkUnstableBranch(n, Snum(typename Snum::NumberType(0.)));
    if (n.number >= 0.)
    {
        return n;
    }
    else
    {
        return -n;
    }
};
using Sstd::abs;

// fabs
templated inline const Snum Sstd::fabs(const Snum& n)
{
    return Sstd::abs(n);
};
using Sstd::fabs;

// min
templated inline const Snum Sstd::min(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
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
templated inline const Snum Sstd::max(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
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

// fmin
templated inline const Snum Sstd::fmin(const Snum& n1, const Snum& n2)
{
    return Sstd::min(n1, n2);
};
set_Sfunction2_casts(fmin);

// fmax
templated inline const Snum Sstd::fmax(const Snum& n1, const Snum& n2)
{
    return Sstd::max(n1, n2);
};
set_Sfunction2_casts(fmax);

// copysign
templated inline const Snum Sstd::copysign(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n2, Snum(typename Snum::NumberType(0.)));

    numberType newNumber = std::copysign(n1.number, n2.number);
    if (newNumber == n1.number) // TODO does not deal properly with Nan
    {
        return n1;
    }
    else
    {
        return -n1;
    }
};
set_Sfunction2_casts(copysign);

//-----------------------------------------------------------------------------
// LINEARISABLE FUNCTIONS

// sqrt
templated const Snum Sstd::sqrt(const Snum& n)
{
    numberType result = std::sqrt(n.number);
    errorType newError;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if (result == 0)
        {
            if(n.error == 0)
            {
                newError = 0;
                newErrorComp = Serror();
            }
            else
            {
                newError = (errorType) std::sqrt((preciseType) std::abs(n.error));
                newErrorComp = Serror(n.errorComposants);
                newErrorComp.multByScalar(newError / n.error);
            }
        }
        else
        {
            numberType remainder = EFT::RemainderSqrt(n.number, result);
            newError = (remainder + n.error) / (result + result);

            newErrorComp = Serror(remainder);
            newErrorComp.addErrors(n.errorComposants);
            newErrorComp.divByScalar(result + result);
        }
        return Snum(result, newError, newErrorComp);
    #else
        if (result == 0)
        {
            if(n.error == 0)
            {
                newError = 0;
            }
            else
            {
                newError = (errorType) std::sqrt((preciseType) std::abs(n.error));
            }
        }
        else
        {
            numberType remainder = EFT::RemainderSqrt(n.number, result);
            newError = (remainder + n.error) / (result + result);
        }
        return Snum(result, newError);
    #endif
};
using Sstd::sqrt;

// fma
templated const Snum Sstd::fma(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::fma(n1.number, n2.number, n3.number);

    numberType remainder = EFT::ErrorFma(n1.number, n2.number, n3.number, result);
    //errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error) + n3.error;
    errorType newError = std::fma(n2.number, n1.error, std::fma(n1.number, n2.error, remainder + n3.error));

    #ifdef TAGGED_ERROR
        Serror newErrorComp(remainder);
        newErrorComp.addErrorsTimeScalar(n2.errorComposants, n1.number);
        newErrorComp.addErrorsTimeScalar(n1.errorComposants, n2.number);
        newErrorComp.addErrors(n3.errorComposants);
        return Snum(result, newError, newErrorComp);
    #else
        return Snum(result, newError);
    #endif
};
set_Sfunction3_casts(fma);

//-----------------------------------------------------------------------------
// GENERAL FUNCTIONS

// TODO macro that turns a function into a src function
#ifdef TAGGED_ERROR
    #define SHAMAN_FUNCTION(functionName) \
    templated const Snum Sstd::functionName (const Snum& n) \
    { \
        numberType result = std::functionName(n.number); \
        preciseType preciseCorrectedResult = std::functionName(n.corrected_number()); \
        preciseType totalError = preciseCorrectedResult - result; \
        Serror newErrorComp; \
        if(n.error == 0.) \
        { \
            newErrorComp = Serror(totalError); \
        } \
        else \
        { \
            preciseType preciseResult = std::functionName((preciseType)n.number); \
            preciseType functionError = preciseResult - result; \
            preciseType proportionalInputError = (totalError - functionError) / n.error; \
            newErrorComp = Serror(functionError); \
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError); \
        } \
        return Snum(result, totalError, newErrorComp); \
    } \
    using Sstd::functionName;
#else
    #define SHAMAN_FUNCTION(functionName) \
    templated const Snum Sstd::functionName (const Snum& n) \
    { \
        numberType result = std::functionName(n.number); \
        preciseType preciseCorrectedResult = std::functionName(n.corrected_number()); \
        preciseType totalError = preciseCorrectedResult - result; \
        return Snum(result, totalError); \
    } \
    using Sstd::functionName;
#endif

SHAMAN_FUNCTION(floor);
SHAMAN_FUNCTION(ceil);
SHAMAN_FUNCTION(trunc);
SHAMAN_FUNCTION(cbrt);
SHAMAN_FUNCTION(exp);
SHAMAN_FUNCTION(exp2);
SHAMAN_FUNCTION(erf);
SHAMAN_FUNCTION(sin);
SHAMAN_FUNCTION(sinh);
SHAMAN_FUNCTION(asinh);
SHAMAN_FUNCTION(cos);
SHAMAN_FUNCTION(cosh);
SHAMAN_FUNCTION(atan);
SHAMAN_FUNCTION(tan);
SHAMAN_FUNCTION(tanh);

//-----------------------------------------------------------------------------
// CONSTRAINED FUNCTIONS

// log
templated const Snum Sstd::log(const Snum& n)
{
    numberType result = std::log(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log(correctedNumber);
    }
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::log((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::log;

// log2
templated const Snum Sstd::log2(const Snum& n)
{
    numberType result = std::log2(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log2(correctedNumber);
    }
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::log2((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::log2;

// log10
templated const Snum Sstd::log10(const Snum& n)
{
    numberType result = std::log10(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log10(correctedNumber);
    }
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::log10((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::log10;

// logb
templated const Snum Sstd::logb(const Snum& n)
{
    numberType result = std::logb(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber == 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::logb(correctedNumber);
    }
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::logb((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::logb;

// acosh
templated const Snum Sstd::acosh(const Snum& n)
{
    numberType result = std::acosh(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 1.)
    {
        preciseCorrectedResult = 0.;
    }
    else
    {
        preciseCorrectedResult = std::acosh(correctedNumber);
    }
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::acosh((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::acosh;

// acos
templated const Snum Sstd::acos(const Snum& n)
{
    numberType result = std::acos(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
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
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::acos((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::acos;

// asin
templated const Snum Sstd::asin(const Snum& n)
{
    numberType result = std::asin(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
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
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::asin((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::asin;

// atanh
templated const Snum Sstd::atanh(const Snum& n)
{
    numberType result = std::atanh(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if(correctedNumber > 1. || correctedNumber < 1.)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::atanh(correctedNumber);
    }
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::asin((preciseType)n.number);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::atanh;

//-----------------------------------------------------------------------------
// MULTIARGUMENTS FUNCTIONS

/*
 * we redistribute the error in a way that is proportionnal to the impact of each argument
 * while keeping the sum of error correct
 */

// scalbn
templated const Snum Sstd::scalbn(const Snum &n, int power)
{
    numberType result = std::scalbn(n.number, power);
    preciseType preciseCorrectedResult = std::scalbn(n.corrected_number(), power);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::scalbn((preciseType)n.number, power);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::scalbn;

// frexp
templated const Snum Sstd::frexp(const Snum& n, int* exp)
{
    numberType result = std::frexp(n.number, exp);
    int dummyExp; // a pointer integer in which to store the result, it can be safely discarded
    preciseType preciseCorrectedResult = std::frexp(n.corrected_number(), &dummyExp);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::frexp((preciseType)n.number, &dummyExp);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::frexp;

// ldexp
templated const Snum Sstd::ldexp(const Snum& n, int exp)
{
    numberType result = std::ldexp(n.number, exp);
    preciseType preciseCorrectedResult = std::ldexp(n.corrected_number(), exp);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        if(n.error == 0)
        {
            newErrorComp = Serror(totalError);
        }
        else
        {
            preciseType preciseResult = std::ldexp((preciseType)n.number, exp);
            preciseType functionError = preciseResult - result;
            preciseType proportionalInputError = (totalError - functionError) / n.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
using Sstd::ldexp;

// pow
templated const Snum Sstd::pow(const Snum& n1, const Snum& n2)
{
    numberType result = std::pow(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::pow(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        preciseType preciseResult = std::pow((preciseType)n1.number, (preciseType)n2.number);
        preciseType functionError = preciseResult - result;
        if((n1.error == 0.) && (n2.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if (n2.error == 0.)
        {
            preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
        }
        else if (n1.error == 0.)
        {
            preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        else
        {
            preciseType preciseCorrectedBut1Result = std::pow((preciseType)n1, n2.corrected_number());
            preciseType preciseCorrectedBut2Result = std::pow(n1.corrected_number(), (preciseType)n2);
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(pow);

// atan2
templated const Snum Sstd::atan2(const Snum& n1, const Snum& n2)
{
    numberType result = std::atan2(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::atan2(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        preciseType preciseResult = std::atan2((preciseType)n1.number, (preciseType)n2.number);
        preciseType functionError = preciseResult - result;
        if((n1.error == 0.) && (n2.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if (n2.error == 0.)
        {
            preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
        }
        else if (n1.error == 0.)
        {
            preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        else
        {
            preciseType preciseCorrectedBut1Result = std::atan2((preciseType)n1, n2.corrected_number());
            preciseType preciseCorrectedBut2Result = std::atan2(n1.corrected_number(), (preciseType)n2);
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(atan2);

// hypot
templated const Snum Sstd::hypot(const Snum& n1, const Snum& n2)
{
    numberType result = std::hypot(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        preciseType preciseResult = std::hypot((preciseType)n1.number, (preciseType)n2.number);
        preciseType functionError = preciseResult - result;
        if((n1.error == 0.) && (n2.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if (n2.error == 0.)
        {
            preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
        }
        else if (n1.error == 0.)
        {
            preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        else
        {
            preciseType preciseCorrectedBut1Result = std::hypot((preciseType)n1, n2.corrected_number());
            preciseType preciseCorrectedBut2Result = std::hypot(n1.corrected_number(), (preciseType)n2);
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(hypot);

// hypot
templated const Snum Sstd::hypot(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::hypot(n1.number, n2.number, n3.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number(), n3.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef TAGGED_ERROR
        Serror newErrorComp;
        preciseType preciseResult = std::hypot((preciseType)n1.number, (preciseType)n2.number, (preciseType)n3.number);
        preciseType preciseCorrectedBut1Result = std::hypot((preciseType)n1, n2.corrected_number(), n3.corrected_number());
        preciseType preciseCorrectedBut2Result = std::hypot(n1.corrected_number(), (preciseType)n2, n3.corrected_number());
        preciseType preciseCorrectedBut3Result = std::hypot(n1.corrected_number(), n2.corrected_number(), (preciseType)n3);
        preciseType functionError = preciseResult - result;
        if((n1.error == 0.) && (n2.error == 0.) && (n3.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if ((n2.error == 0.) && (n3.error == 0.))
        {
            preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
        }
        else if ((n1.error == 0.) && (n3.error == 0.))
        {
            preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        else if ((n1.error == 0.) && (n2.error == 0.))
        {
            preciseType proportionalInput3Error = (totalError - functionError) / n3.error;
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n3.errorComposants, proportionalInput3Error);
        }
        else if (n3.error == 0)
        {
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        else if (n2.error == 0)
        {
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input3Error = preciseCorrectedResult - preciseCorrectedBut3Result;
            preciseType proportionality = (totalError - functionError) / (input1Error + input3Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput3Error = proportionality * (input3Error / n3.error);
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
            newErrorComp.addErrorsTimeScalar(n3.errorComposants, proportionalInput3Error);
        }
        else if (n1.error == 0)
        {
            preciseType input3Error = preciseCorrectedResult - preciseCorrectedBut3Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType proportionality = (totalError - functionError) / (input3Error + input2Error);
            preciseType proportionalInput3Error = proportionality * (input3Error / n3.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n3.errorComposants, proportionalInput3Error);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
        }
        else
        {
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType input3Error = preciseCorrectedResult - preciseCorrectedBut3Result;
            preciseType proportionality = (totalError - functionError) / (input1Error + input2Error + input3Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            preciseType proportionalInput3Error = proportionality * (input3Error / n3.error);
            newErrorComp = Serror(functionError);
            newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
            newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
            newErrorComp.addErrorsTimeScalar(n3.errorComposants, proportionalInput3Error);
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction3_casts(hypot);

//-----------------------------------------------------------------------------

#undef set_Sfunction2_casts
#undef set_Sfunction3_casts

