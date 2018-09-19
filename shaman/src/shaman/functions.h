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
namespace ShamanMaths \
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

// defines overload for function taking three arguments
#define set_Sfunction3_casts(FUN) \
namespace ShamanMaths \
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

//-----------------------------------------------------------------------------
// TEST BASED FUNCTIONS

// isfinite
templated inline bool ShamanMaths::isfinite(const Snum& n)
{
    return std::isfinite(n.number);
};

// isnan
templated inline bool ShamanMaths::isnan(const Snum& n)
{
    return std::isnan(n.number);
};

// abs
templated inline const Snum ShamanMaths::abs(const Snum& n)
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

// fabs
templated inline const Snum ShamanMaths::fabs(const Snum& n)
{
    return ShamanMaths::abs(n);
};

// min
templated inline const Snum ShamanMaths::min(const Snum& n1, const Snum& n2)
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
templated inline const Snum ShamanMaths::max(const Snum& n1, const Snum& n2)
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
templated inline const Snum ShamanMaths::fmin(const Snum& n1, const Snum& n2)
{
    return ShamanMaths::min(n1, n2);
};
set_Sfunction2_casts(fmin);

// fmax
templated inline const Snum ShamanMaths::fmax(const Snum& n1, const Snum& n2)
{
    return ShamanMaths::max(n1, n2);
};
set_Sfunction2_casts(fmax);

//-----------------------------------------------------------------------------
// LINEARISABLE FUNCTIONS

// sqrt
templated const Snum ShamanMaths::sqrt(const Snum& n)
{
    numberType result = std::sqrt(n.number);

    errorType newError;
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
};

// fma
templated const Snum ShamanMaths::fma(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::fma(n1.number, n2.number, n3.number);

    numberType remainder = EFT::ErrorFma(n1.number, n2.number, n3.number, result);
    //errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error) + n3.error;
    errorType newError = std::fma(n2.number, n1.error, std::fma(n1.number, n2.error, remainder + n3.error));

    Serror newErrorComp(remainder);
    newErrorComp.addErrorsTimeScalar(n2.errorComposants, n1.number);
    newErrorComp.addErrorsTimeScalar(n1.errorComposants, n2.number);
    newErrorComp.addErrors(n3.errorComposants);

    return Snum(result, newError, newErrorComp);
};
set_Sfunction3_casts(fma);

//-----------------------------------------------------------------------------
// GENERAL FUNCTIONS

// macro that turns a function into a src function
#define SHAMAN_FUNCTION(functionName) \
templated const Snum ShamanMaths::functionName (const Snum& n) \
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
templated const Snum ShamanMaths::log(const Snum& n)
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

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// log2
templated const Snum ShamanMaths::log2(const Snum& n)
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

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// log10
templated const Snum ShamanMaths::log10(const Snum& n)
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

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// acosh
templated const Snum ShamanMaths::acosh(const Snum& n)
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

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// acos
templated const Snum ShamanMaths::acos(const Snum& n)
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

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// asin
templated const Snum ShamanMaths::asin(const Snum& n)
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

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// atanh
templated const Snum ShamanMaths::atanh(const Snum& n)
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

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

//-----------------------------------------------------------------------------
// MULTIARGUMENTS FUNCTIONS

/*
 * we redistribute the error in a way that is proportionnal to the impact of each argument
 * while keeping the sum of error correct
 */

// scalbn
templated const Snum ShamanMaths::scalbn(const Snum &n, int power)
{
    numberType result = std::scalbn(n.number, power);
    preciseType preciseCorrectedResult = std::scalbn(n.corrected_number(), power);

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// frexp
templated const Snum ShamanMaths::frexp(const Snum& n, int* exp)
{
    numberType result = std::frexp(n.number, exp);
    int dummyExp; // a pointer integer in which to store the result, it can be safely discarded
    preciseType preciseCorrectedResult = std::frexp(n.corrected_number(), &dummyExp);

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// ldexp
templated const Snum ShamanMaths::ldexp(const Snum& n, int exp)
{
    numberType result = std::ldexp(n.number, exp);
    preciseType preciseCorrectedResult = std::ldexp(n.corrected_number(), exp);

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};

// pow
templated const Snum ShamanMaths::pow(const Snum& n1, const Snum& n2)
{
    numberType result = std::pow(n1.number, n2.number);
    preciseType preciseResult = std::pow((preciseType)n1.number, (preciseType)n2.number);
    preciseType preciseCorrectedResult = std::pow(n1.corrected_number(), n2.corrected_number());

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};
set_Sfunction2_casts(pow);

// atan2
templated const Snum ShamanMaths::atan2(const Snum& n1, const Snum& n2)
{
    numberType result = std::atan2(n1.number, n2.number);
    preciseType preciseResult = std::atan2((preciseType)n1.number, (preciseType)n2.number);
    preciseType preciseCorrectedResult = std::atan2(n1.corrected_number(), n2.corrected_number());

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};
set_Sfunction2_casts(atan2);

// hypot
templated const Snum ShamanMaths::hypot(const Snum& n1, const Snum& n2)
{
    numberType result = std::hypot(n1.number, n2.number);
    preciseType preciseResult = std::hypot((preciseType)n1.number, (preciseType)n2.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number());

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};
set_Sfunction2_casts(hypot);

// hypot
templated const Snum ShamanMaths::hypot(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::hypot(n1.number, n2.number, n3.number);
    preciseType preciseResult = std::hypot((preciseType)n1.number, (preciseType)n2.number, (preciseType)n3.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number(), n3.corrected_number());
    preciseType preciseCorrectedBut1Result = std::hypot((preciseType)n1, n2.corrected_number(), n3.corrected_number());
    preciseType preciseCorrectedBut2Result = std::hypot(n1.corrected_number(), (preciseType)n2, n3.corrected_number());
    preciseType preciseCorrectedBut3Result = std::hypot(n1.corrected_number(), n2.corrected_number(), (preciseType)n3);

    Serror newErrorComp;
    preciseType totalError = preciseCorrectedResult - result;
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
};
set_Sfunction3_casts(hypot);

//-----------------------------------------------------------------------------

#undef set_Sfunction2_casts
#undef set_Sfunction3_casts

