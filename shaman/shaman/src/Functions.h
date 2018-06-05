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
// TEST BASED FUNCTIONS

// isfinite
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
    Snum::checkUnstableBranch(n, 0.);
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
templated inline const Snum fabs(const Snum& n)
{
    return abs(n);
};

// min
templated inline const Snum min(const Snum& n1, const Snum& n2)
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
templated inline const Snum max(const Snum& n1, const Snum& n2)
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

//-----------------------------------------------------------------------------
// LINEARISABLE FUNCTIONS

// sqrt
templated inline const Snum sqrt(const Snum& n)
{
    numberType result = std::sqrt(n.number);

    errorType newError;
    Serror newErrorComp;
    if (result == 0)
    {
        newError = (errorType) std::sqrt((preciseType) std::abs(n.error));
        newErrorComp = Serror(n.errorComposants);
        newErrorComp.multByScalar(newError / n.error);
    }
    else
    {
        numberType remainder = EFT::RemainderSqrt(n.number, result);
        newError = (remainder + n.error) / (result + result);

        newErrorComp = Serror(remainder);
        newErrorComp.addError(n.errorComposants);
        newErrorComp.divByScalar(result + result);
    }

    return Snum(result, newError, newErrorComp);
};

// fma
templated inline const Snum fma(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::fma(n1.number, n2.number, n3.number);

    numberType remainder = EFT::ErrorFma(n1.number, n2.number, n3.number, result);
    //errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error) + n3.error;
    errorType newError = std::fma(n2.number, n1.error, std::fma(n1.number, n2.error, remainder + n3.error));

    Serror newErrorComp = Serror(remainder);
    newErrorComp.addErrorsTimeScalar(n2.errorComposants, n1.number);
    newErrorComp.addErrorsTimeScalar(n1.errorComposants, n2.number);
    newErrorComp.addError(n3.errorComposants);

    return Snum(result, newError, newErrorComp);
};
set_Sfunction3_casts(fma);

//-----------------------------------------------------------------------------
// GENERAL FUNCTIONS

/*
 * functions that computes the error of a function
 * given the function for the base precision, a more precise version and the argument of the function
 */
template<typename FUNnumberType, typename FUNpreciseType, typename numberType, typename errorType, typename preciseType>
inline const Snum general_function(FUNnumberType fn, FUNpreciseType fp, const Snum& n)
{
    numberType result = fn(n.number);
    preciseType preciseResult = fp(n.number);
    preciseType preciseCorrectedResult = fp(n.corrected_number());

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// macro that turns a function into a shaman function
#define SHAMAN_FUNCTION(functionName) \
templated inline const Snum functionName(const Snum& argument) \
{ \
    return general_function(functionName<numberType>, functionName<preciseType>, argument); \
} \

SHAMAN_FUNCTION(std::floor);
SHAMAN_FUNCTION(std::ceil);
SHAMAN_FUNCTION(std::trunc);
SHAMAN_FUNCTION(std::cbrt);
SHAMAN_FUNCTION(std::exp);
SHAMAN_FUNCTION(std::exp2);
SHAMAN_FUNCTION(std::erf);
SHAMAN_FUNCTION(std::sin);
SHAMAN_FUNCTION(std::sinh);
SHAMAN_FUNCTION(std::asinh);
SHAMAN_FUNCTION(std::cos);
SHAMAN_FUNCTION(std::cosh);
SHAMAN_FUNCTION(std::atan);
SHAMAN_FUNCTION(std::tan);
SHAMAN_FUNCTION(std::tanh);

//-----------------------------------------------------------------------------
// CONSTRAINED FUNCTIONS

// log
templated inline const Snum log(const Snum& n)
{
    numberType result = std::log<numberType>(n.number);
    preciseType preciseResult = std::log<preciseType>(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log<preciseType>(correctedNumber);
    }

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// log2
templated inline const Snum log2(const Snum& n)
{
    numberType result = std::log2<numberType>(n.number);
    preciseType preciseResult = std::log2<preciseType>(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log2<preciseType>(correctedNumber);
    }

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// log10
templated inline const Snum log10(const Snum& n)
{
    numberType result = std::log10<numberType>(n.number);
    preciseType preciseResult = std::log10<preciseType>(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 0)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::log10<preciseType>(correctedNumber);
    }

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// acosh
templated inline const Snum acosh(const Snum& n)
{
    numberType result = std::acosh<numberType>(n.number);
    preciseType preciseResult = std::acosh<preciseType>(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if (correctedNumber < 1.)
    {
        preciseCorrectedResult = 0.;
    }
    else
    {
        preciseCorrectedResult = std::acosh<preciseType>(correctedNumber);
    }

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// acos
templated inline const Snum acos(const Snum& n)
{
    numberType result = std::acos<numberType>(n.number);
    preciseType preciseResult = std::acos<preciseType>(n.number);

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
        preciseCorrectedResult = std::acos<preciseType>(correctedNumber);
    }

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// asin
templated inline const Snum asin(const Snum& n)
{
    numberType result = std::asin<numberType>(n.number);
    preciseType preciseResult = std::asin<preciseType>(n.number);

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
        preciseCorrectedResult = std::asin<preciseType>(correctedNumber);
    }

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// atanh
templated inline const Snum atanh(const Snum& n)
{
    numberType result = std::atanh<numberType>(n.number);
    preciseType preciseResult = std::asin<preciseType>(n.number);

    preciseType preciseCorrectedResult;
    preciseType correctedNumber = n.corrected_number();
    if(correctedNumber > 1. || correctedNumber < 1.)
    {
        preciseCorrectedResult = -INFINITY;
    }
    else
    {
        preciseCorrectedResult = std::atanh<preciseType>(correctedNumber);
    }

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

//-----------------------------------------------------------------------------
// MULTIARGUMENTS FUNCTIONS

/*
 * we redistribute the error in a way that is proportionnal to the impact of each argument
 * while keeping the sum of error correct
 */

// frexp
templated inline const Snum frexp(const Snum& n, int* exp)
{
    numberType result = std::frexp<numberType>(n.number, exp);
    int dummyExp; // a pointer integer in which to store the result, it can be safely discarded
    preciseType preciseResult = std::frexp<preciseType>(n.number, &dummyExp);
    preciseType preciseCorrectedResult = std::frexp<preciseType>(n.corrected_number(), &dummyExp);

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// ldexp
templated inline const Snum ldexp(const Snum& n, int exp)
{
    numberType result = std::ldexp<numberType>(n.number, exp);
    preciseType preciseResult = std::ldexp<preciseType>(n.number, exp);
    preciseType preciseCorrectedResult = std::ldexp<preciseType>(n.corrected_number(), exp);

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType proportionalInputError = (totalError - functionError) / n.error;

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n.errorComposants, proportionalInputError);

    return Snum(result, totalError, newErrorComp);
};

// pow
templated inline const Snum pow(const Snum& n1, const Snum& n2)
{
    numberType result = std::pow<numberType>(n1.number, n2.number);
    preciseType preciseResult = std::pow<preciseType>(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::pow<preciseType>(n1.corrected_number(), n2.corrected_number());
    preciseType preciseCorrectedBut1Result = std::pow<preciseType>(n1, n2.corrected_number());
    preciseType preciseCorrectedBut2Result = std::pow<preciseType>(n1.corrected_number(), n2);

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
    preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;

    preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
    newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);

    return Snum(result, totalError, newErrorComp);
};
set_Sfunction2_casts(pow);

// atan2
templated inline const Snum atan2(const Snum& n1, const Snum& n2)
{
    numberType result = std::atan2<numberType>(n1.number, n2.number);
    preciseType preciseResult = std::atan2<preciseType>(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::atan2<preciseType>(n1.corrected_number(), n2.corrected_number());
    preciseType preciseCorrectedBut1Result = std::atan2<preciseType>(n1, n2.corrected_number());
    preciseType preciseCorrectedBut2Result = std::atan2<preciseType>(n1.corrected_number(), n2);

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
    preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;

    preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
    newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);

    return Snum(result, totalError, newErrorComp);
};
set_Sfunction2_casts(atan2);

// hypot
templated inline const Snum hypot(const Snum& n1, const Snum& n2)
{
    numberType result = std::hypot<numberType>(n1.number, n2.number);
    preciseType preciseResult = std::hypot<preciseType>(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::hypot<preciseType>(n1.corrected_number(), n2.corrected_number());
    preciseType preciseCorrectedBut1Result = std::hypot<preciseType>(n1, n2.corrected_number());
    preciseType preciseCorrectedBut2Result = std::hypot<preciseType>(n1.corrected_number(), n2);

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
    preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;

    preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
    newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);

    return Snum(result, totalError, newErrorComp);
};
set_Sfunction2_casts(hypot);

// hypot
templated inline const Snum hypot(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::hypot<numberType>(n1.number, n2.number, n3.number);
    preciseType preciseResult = std::hypot<preciseType>(n1.number, n2.number, n3.number);
    preciseType preciseCorrectedResult = std::hypot<preciseType>(n1.corrected_number(), n2.corrected_number(), n3.corrected_number());
    preciseType preciseCorrectedBut1Result = std::hypot<preciseType>(n1, n2.corrected_number(), n3.corrected_number());
    preciseType preciseCorrectedBut2Result = std::hypot<preciseType>(n1.corrected_number(), n2, n3.corrected_number());
    preciseType preciseCorrectedBut3Result = std::hypot<preciseType>(n1.corrected_number(), n2.corrected_number(), n3);

    preciseType totalError = preciseCorrectedResult - result;
    preciseType functionError = preciseResult - result;
    preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
    preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
    preciseType input3Error = preciseCorrectedResult - preciseCorrectedBut3Result;

    preciseType proportionality = (totalError - functionError) / (input1Error + input2Error + input3Error);
    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
    preciseType proportionalInput3Error = proportionality * (input3Error / n3.error);

    Serror newErrorComp = Serror(functionError);
    newErrorComp.addErrorsTimeScalar(n1.errorComposants, proportionalInput1Error);
    newErrorComp.addErrorsTimeScalar(n2.errorComposants, proportionalInput2Error);
    newErrorComp.addErrorsTimeScalar(n3.errorComposants, proportionalInput3Error);

    return Snum(result, totalError, newErrorComp);
};
set_Sfunction3_casts(hypot);

//-----------------------------------------------------------------------------

#undef set_Sfunction2_casts
#undef set_Sfunction3_casts

#endif //SHAMAN_FUNCTIONS_H