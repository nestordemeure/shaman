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
    }

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
    }

#ifdef SHAMAN_TAGGED_ERROR
#define SHAMAN_FUNCTION(functionName) \
    templated const Snum functionName (const Snum& n) \
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
            newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;}); \
            newErrorComp.addError(functionError); \
        } \
        return Snum(result, totalError, newErrorComp); \
    }
#else
    #define SHAMAN_FUNCTION(functionName) \
    templated const Snum functionName (const Snum& n) \
    { \
        numberType result = std::functionName(n.number); \
        preciseType preciseCorrectedResult = std::functionName(n.corrected_number()); \
        preciseType totalError = preciseCorrectedResult - result; \
        return Snum(result, totalError); \
    }
#endif

//-----------------------------------------------------------------------------
// Sstd DEFINITION

#ifdef NO_SHAMAN
namespace Sstd = std;
#else
namespace Sstd
{
using namespace std;
using namespace Shaman;

// ---------- TRIGONOMETRIC FUNCTIONS ----------

SHAMAN_FUNCTION(cos);
SHAMAN_FUNCTION(sin);
SHAMAN_FUNCTION(tan);
SHAMAN_FUNCTION(atan);

// acos
templated const Snum acos(const Snum& n)
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

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
            return Snum(result, totalError);
    #endif
};

// asin
templated const Snum asin(const Snum& n)
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

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
            return Snum(result, totalError);
    #endif
};

// atan2
templated const Snum atan2(const Snum& n1, const Snum& n2)
{
    numberType result = std::atan2(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::atan2(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
                newErrorComp.addError(functionError);
            }
            else if (n1.error == 0.)
            {
                preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
                newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
                newErrorComp.addError(functionError);
            }
            else
            {
                preciseType preciseCorrectedBut1Result = std::atan2((preciseType)n1, n2.corrected_number());
                preciseType preciseCorrectedBut2Result = std::atan2(n1.corrected_number(), (preciseType)n2);
                preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
                preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
                preciseType inputError = input1Error + input2Error;
                if (inputError == 0.)
                {
                    // avoid division by zero
                    newErrorComp = Serror(functionError);
                }
                else
                {
                    preciseType proportionality = (totalError - functionError) / inputError;
                    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                    newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
                    newErrorComp.addError(functionError);
                }
            }
            return Snum(result, totalError, newErrorComp);
    #else
            return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(atan2);

// ---------- HYPERBOLIC FUNCTIONS ----------

SHAMAN_FUNCTION(cosh);
SHAMAN_FUNCTION(sinh);
SHAMAN_FUNCTION(tanh);
SHAMAN_FUNCTION(asinh);

// acosh
templated const Snum acosh(const Snum& n)
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

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// atanh
templated const Snum atanh(const Snum& n)
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

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// ---------- EXPONENTIAL AND LOGARITHMIC FUNCTIONS ----------

SHAMAN_FUNCTION(exp);
SHAMAN_FUNCTION(exp2);
SHAMAN_FUNCTION(expm1);
SHAMAN_FUNCTION(ilogb);

// frexp
templated const Snum frexp(const Snum& n, int* exp)
{
    numberType result = std::frexp(n.number, exp);
    int dummyExp; // a pointer integer in which to store the result, it can be safely discarded
    preciseType preciseCorrectedResult = std::frexp(n.corrected_number(), &dummyExp);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// ldexp
templated const Snum ldexp(const Snum& n, int exp)
{
    numberType result = std::ldexp(n.number, exp);
    preciseType preciseCorrectedResult = std::ldexp(n.corrected_number(), exp);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// log
templated const Snum log(const Snum& n)
{
    numberType result = std::log(n.number);

    preciseType totalError;
    preciseType correctedNumber = n.corrected_number();
    if((correctedNumber <= 0.) && (n.number <= 0.))
    {
        // resolves the case inf-inf
        totalError = 0.;
    }
    else
    {
        preciseType preciseCorrectedResult;
        if (correctedNumber < 0)
        {
            preciseCorrectedResult = -INFINITY;
        }
        else
        {
            preciseCorrectedResult = std::log(correctedNumber);
        }
        totalError = preciseCorrectedResult - result;
    }

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// log10
templated const Snum log10(const Snum& n)
{
    numberType result = std::log10(n.number);

    preciseType totalError;
    preciseType correctedNumber = n.corrected_number();
    if((correctedNumber <= 0.) && (n.number <= 0.))
    {
        // resolves the case inf-inf
        totalError = 0.;
    }
    else
    {
        preciseType preciseCorrectedResult;
        if (correctedNumber < 0)
        {
            preciseCorrectedResult = -INFINITY;
        }
        else
        {
            preciseCorrectedResult = std::log10(correctedNumber);
        }
        totalError = preciseCorrectedResult - result;
    }

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// modf
templated const Snum modf(const Snum& n, Snum* intpart)
{
    numberType intpartNumber;
    numberType fractPartNumber = std::modf(n.number, &intpartNumber);
    preciseType intpartPrecise;
    preciseType fractPartPrecise = std::modf(n.corrected_number(), &intpartPrecise);

    preciseType fractTotalError = fractPartPrecise - fractPartNumber;
    preciseType intTotalError = intpartPrecise - intpartNumber;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror fractErrorComp;
        Serror intErrorComp;
        if(n.error == 0)
        {
            fractErrorComp = Serror(fractTotalError);
            intErrorComp = Serror(intTotalError);
        }
        else
        {
            preciseType intPreciseResult;
            preciseType fractPreciseResult = std::modf((preciseType)n.number, &intPreciseResult);

            preciseType intFunctionError = intPreciseResult - intpartNumber;
            preciseType fractFunctionError = fractPreciseResult - fractPartNumber;

            preciseType intProportionalInputError = (intTotalError - intFunctionError) / n.error;
            preciseType fractProportionalInputError = (fractTotalError - fractFunctionError) / n.error;

            intErrorComp = Serror(n.errorComposants, [intProportionalInputError](errorType e){return e*intProportionalInputError;});
            intErrorComp.addError(intFunctionError);
            fractErrorComp = Serror(n.errorComposants, [fractProportionalInputError](errorType e){return e*fractProportionalInputError;});
            fractErrorComp.addError(fractFunctionError);
        }
        (*intpart).number = intpartNumber;
        (*intpart).error = intTotalError;
        (*intpart).errorComposants = intErrorComp;
        return Snum(fractPartNumber, fractTotalError, fractErrorComp);
    #else
        (*intpart).number = intpartNumber;
        (*intpart).error = intTotalError;
        return Snum(fractPartNumber, fractTotalError);
    #endif
};

// log1p
templated const Snum log1p(const Snum& n)
{
    numberType result = std::log1p(n.number);

    preciseType totalError;
    preciseType correctedNumber = n.corrected_number();
    if((correctedNumber <= -1.) && (n.number <= -1.))
    {
        // resolves the case inf-inf
        totalError = 0.;
    }
    else
    {
        preciseType preciseCorrectedResult;
        if (correctedNumber < -1.)
        {
            preciseCorrectedResult = -INFINITY;
        }
        else
        {
            preciseCorrectedResult = std::log1p(correctedNumber);
        }
        totalError = preciseCorrectedResult - result;
    }

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
            if(n.error == 0)
            {
                newErrorComp = Serror(totalError);
            }
            else
            {
                preciseType preciseResult = std::log1p((preciseType)n.number);
                preciseType functionError = preciseResult - result;
                preciseType proportionalInputError = (totalError - functionError) / n.error;
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// log2
templated const Snum log2(const Snum& n)
{
    numberType result = std::log2(n.number);

    preciseType totalError;
    preciseType correctedNumber = n.corrected_number();
    if((correctedNumber <= 0.) && (n.number <= 0.))
    {
        // resolves the case inf-inf
        totalError = 0.;
    }
    else
    {
        preciseType preciseCorrectedResult;
        if (correctedNumber < 0)
        {
            preciseCorrectedResult = -INFINITY;
        }
        else
        {
            preciseCorrectedResult = std::log2(correctedNumber);
        }
        totalError = preciseCorrectedResult - result;
    }

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// logb
templated const Snum logb(const Snum& n)
{
    numberType result = std::logb(n.number);

    preciseType totalError;
    preciseType correctedNumber = n.corrected_number();
    if((correctedNumber <= 0.) && (n.number <= 0.))
    {
        // resolves the case inf-inf
        totalError = 0.;
    }
    else
    {
        preciseType preciseCorrectedResult;
        if (correctedNumber < 0)
        {
            preciseCorrectedResult = -INFINITY;
        }
        else
        {
            preciseCorrectedResult = std::logb(correctedNumber);
        }
        totalError = preciseCorrectedResult - result;
    }

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// scalbn
templated const Snum scalbn(const Snum &n, int power)
{
    numberType result = std::scalbn(n.number, power);
    preciseType preciseCorrectedResult = std::scalbn(n.corrected_number(), power);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// scalbln
templated const Snum scalbln(const Snum &n, long int power)
{
    numberType result = std::scalbln(n.number, power);
    preciseType preciseCorrectedResult = std::scalbln(n.corrected_number(), power);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
            if(n.error == 0)
            {
                newErrorComp = Serror(totalError);
            }
            else
            {
                preciseType preciseResult = std::scalbln((preciseType)n.number, power);
                preciseType functionError = preciseResult - result;
                preciseType proportionalInputError = (totalError - functionError) / n.error;
                newErrorComp = Serror(n.errorComposants, [proportionalInputError](errorType e){return e*proportionalInputError;});
                newErrorComp.addError(functionError);
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// ---------- POWER FUNCTIONS ----------

SHAMAN_FUNCTION(cbrt);

// pow
templated const Snum pow(const Snum& n1, const Snum& n2)
{
    numberType result = std::pow(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::pow(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
                newErrorComp.addError(functionError);
            }
            else if (n1.error == 0.)
            {
                preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
                newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
                newErrorComp.addError(functionError);
            }
            else
            {
                preciseType preciseCorrectedBut1Result = std::pow((preciseType)n1, n2.corrected_number());
                preciseType preciseCorrectedBut2Result = std::pow(n1.corrected_number(), (preciseType)n2);
                preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
                preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
                preciseType inputError = input1Error + input2Error;
                if (inputError == 0.)
                {
                    // avoid division by zero
                    newErrorComp = Serror(functionError);
                }
                else
                {
                    preciseType proportionality = (totalError - functionError) / inputError;
                    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                    newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
                    newErrorComp.addError(functionError);
                }
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(pow);

// sqrt
templated const Snum sqrt(const Snum& n)
{
    numberType result = std::sqrt(n.number);
    errorType newError;

    #ifdef SHAMAN_TAGGED_ERROR
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
                    errorType scaling = newError / n.error;
                    newErrorComp = Serror(n.errorComposants, [scaling](errorType e){return e*scaling;});
                }
            }
            else
            {
                numberType remainder = EFT::RemainderSqrt(n.number, result);
                newError = (remainder + n.error) / (result + result);

                newErrorComp = Serror(n.errorComposants);
                newErrorComp.addError(remainder);
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

// hypot
templated const Snum hypot(const Snum& n1, const Snum& n2)
{
    numberType result = std::hypot(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
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
                newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
                newErrorComp.addError(functionError);
            }
            else if (n1.error == 0.)
            {
                preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
                newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
                newErrorComp.addError(functionError);
            }
            else
            {
                preciseType preciseCorrectedBut1Result = std::hypot((preciseType)n1, n2.corrected_number());
                preciseType preciseCorrectedBut2Result = std::hypot(n1.corrected_number(), (preciseType)n2);
                preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
                preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
                preciseType inputError = input1Error + input2Error;
                if (inputError == 0.)
                {
                    // avoid division by zero
                    newErrorComp = Serror(functionError);
                }
                else
                {
                    preciseType proportionality = (totalError - functionError) / inputError;
                    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                    newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
                    newErrorComp.addError(functionError);
                }
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(hypot);

// hypot
templated const Snum hypot(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::hypot(n1.number, n2.number, n3.number);
    preciseType preciseCorrectedResult = std::hypot(n1.corrected_number(), n2.corrected_number(), n3.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

#ifdef SHAMAN_TAGGED_ERROR
    Serror newErrorComp;
        preciseType preciseResult = std::hypot((preciseType)n1.number, (preciseType)n2.number, (preciseType)n3.number);
        preciseType preciseCorrectedBut1Result = std::hypot((preciseType)n1, n2.corrected_number(), n3.corrected_number());
        preciseType preciseCorrectedBut2Result = std::hypot(n1.corrected_number(), (preciseType)n2, n3.corrected_number());
        preciseType preciseCorrectedBut3Result = std::hypot(n1.corrected_number(), n2.corrected_number(), (preciseType)n3);
        preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
        preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
        preciseType input3Error = preciseCorrectedResult - preciseCorrectedBut3Result;
        preciseType functionError = preciseResult - result;
        if((n1.error == 0.) and (n2.error == 0.) and (n3.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if ((n2.error == 0.) and (n3.error == 0.))
        {
            preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
            newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
            newErrorComp.addError(functionError);
        }
        else if ((n1.error == 0.) and (n3.error == 0.))
        {
            preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
            newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
            newErrorComp.addError(functionError);
        }
        else if ((n1.error == 0.) and (n2.error == 0.))
        {
            preciseType proportionalInput3Error = (totalError - functionError) / n3.error;
            newErrorComp = Serror(n3.errorComposants, [proportionalInput3Error](errorType e){return e*proportionalInput3Error;});
            newErrorComp.addError(functionError);
        }
        else if ((n3.error == 0) and (input1Error + input2Error != 0.))
        {
            preciseType proportionality = (totalError - functionError) / (input1Error + input2Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
            newErrorComp.addError(functionError);
        }
        else if ((n2.error == 0) and (input1Error + input3Error != 0.))
        {
            preciseType proportionality = (totalError - functionError) / (input1Error + input3Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput3Error = proportionality * (input3Error / n3.error);
            newErrorComp = Serror(n1.errorComposants, n3.errorComposants, [proportionalInput1Error, proportionalInput3Error](errorType e1, errorType e3){return e1*proportionalInput1Error + e3*proportionalInput3Error;});
            newErrorComp.addError(functionError);
        }
        else if ((n1.error == 0) and (input2Error + input3Error != 0.))
        {
            preciseType proportionality = (totalError - functionError) / (input3Error + input2Error);
            preciseType proportionalInput3Error = proportionality * (input3Error / n3.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            newErrorComp = Serror(n3.errorComposants, n2.errorComposants, [proportionalInput3Error, proportionalInput2Error](errorType e3, errorType e2){return e3*proportionalInput3Error + e2*proportionalInput2Error;});
            newErrorComp.addError(functionError);
        }
        else if (input1Error + input2Error + input3Error != 0.)
        {
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType input3Error = preciseCorrectedResult - preciseCorrectedBut3Result;
            preciseType proportionality = (totalError - functionError) / (input1Error + input2Error + input3Error);
            preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
            preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
            preciseType proportionalInput3Error = proportionality * (input3Error / n3.error);
            newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
            newErrorComp.addErrorsTimeScalar(n3.errorComposants, proportionalInput3Error);
            newErrorComp.addError(functionError);
        }
        else
        {
        newErrorComp = Serror(functionError);
        }
        return Snum(result, totalError, newErrorComp);
#else
    return Snum(result, totalError);
#endif
};
set_Sfunction3_casts(hypot);

// ---------- ERROR AND GAMMA FUNCTIONS ----------

SHAMAN_FUNCTION(erf);
SHAMAN_FUNCTION(erff);
SHAMAN_FUNCTION(erfl);
SHAMAN_FUNCTION(erfc);
SHAMAN_FUNCTION(erfcf);
SHAMAN_FUNCTION(erfcl);
SHAMAN_FUNCTION(tgamma);
SHAMAN_FUNCTION(lgamma);

// ---------- ROUNDING AND REMAINDER FUNCTIONS ----------

SHAMAN_FUNCTION(ceil);
SHAMAN_FUNCTION(floor);
SHAMAN_FUNCTION(trunc);
SHAMAN_FUNCTION(round);
SHAMAN_FUNCTION(rint);
SHAMAN_FUNCTION(nearbyint);

// fmod
templated const Snum fmod(const Snum& n1, const Snum& n2)
{
    numberType result = std::fmod(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::fmod(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
            preciseType preciseResult = std::fmod((preciseType)n1.number, (preciseType)n2.number);
            preciseType functionError = preciseResult - result;
            if((n1.error == 0.) && (n2.error == 0.))
            {
                newErrorComp = Serror(totalError);
            }
            else if (n2.error == 0.)
            {
                preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
                newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
                newErrorComp.addError(functionError);
            }
            else if (n1.error == 0.)
            {
                preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
                newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
                newErrorComp.addError(functionError);
            }
            else
            {
                preciseType preciseCorrectedBut1Result = std::fmod((preciseType)n1, n2.corrected_number());
                preciseType preciseCorrectedBut2Result = std::fmod(n1.corrected_number(), (preciseType)n2);
                preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
                preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
                preciseType inputError = input1Error + input2Error;
                if (inputError == 0.)
                {
                    // avoid division by zero
                    newErrorComp = Serror(functionError);
                }
                else
                {
                    preciseType proportionality = (totalError - functionError) / inputError;
                    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                    newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
                    newErrorComp.addError(functionError);
                }
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(fmod);

// lround
templated inline const long int lround(const Snum& n)
{
    return std::lround(n.number);
}

// llround
templated inline const long long int llround(const Snum& n)
{
    return std::llround(n.number);
}

// lrint
templated inline const long int lrint(const Snum& n)
{
    return std::lrint(n.number);
}

// llrint
templated inline const long long int llrint(const Snum& n)
{
    return std::llrint(n.number);
}

// remainder
templated const Snum remainder(const Snum& n1, const Snum& n2)
{
    numberType result = std::remainder(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::remainder(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
            preciseType preciseResult = std::remainder((preciseType)n1.number, (preciseType)n2.number);
            preciseType functionError = preciseResult - result;
            if((n1.error == 0.) && (n2.error == 0.))
            {
                newErrorComp = Serror(totalError);
            }
            else if (n2.error == 0.)
            {
                preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
                newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
                newErrorComp.addError(functionError);
            }
            else if (n1.error == 0.)
            {
                preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
                newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
                newErrorComp.addError(functionError);
            }
            else
            {
                preciseType preciseCorrectedBut1Result = std::remainder((preciseType)n1, n2.corrected_number());
                preciseType preciseCorrectedBut2Result = std::remainder(n1.corrected_number(), (preciseType)n2);
                preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
                preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
                preciseType inputError = input1Error + input2Error;
                if (inputError == 0.)
                {
                    // avoid division by zero
                    newErrorComp = Serror(functionError);
                }
                else
                {
                    preciseType proportionality = (totalError - functionError) / inputError;
                    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                    newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
                    newErrorComp.addError(functionError);
                }
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(remainder);

// remquo
templated const Snum remquo(const Snum& n1, const Snum& n2, int* quot)
{
    int dummyquot;
    numberType result = std::remquo(n1.number, n2.number, quot);
    preciseType preciseCorrectedResult = std::remquo(n1.corrected_number(), n2.corrected_number(), &dummyquot);
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
            preciseType preciseResult = std::remquo((preciseType)n1.number, (preciseType)n2.number, &dummyquot);
            preciseType functionError = preciseResult - result;
            if((n1.error == 0.) && (n2.error == 0.))
            {
                newErrorComp = Serror(totalError);
            }
            else if (n2.error == 0.)
            {
                preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
                newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
                newErrorComp.addError(functionError);
            }
            else if (n1.error == 0.)
            {
                preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
                newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
                newErrorComp.addError(functionError);
            }
            else
            {
                preciseType preciseCorrectedBut1Result = std::remquo((preciseType)n1, n2.corrected_number(), &dummyquot);
                preciseType preciseCorrectedBut2Result = std::remquo(n1.corrected_number(), (preciseType)n2, &dummyquot);
                preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
                preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
                preciseType inputError = input1Error + input2Error;
                if (inputError == 0.)
                {
                    // avoid division by zero
                    newErrorComp = Serror(functionError);
                }
                else
                {
                    preciseType proportionality = (totalError - functionError) / inputError;
                    preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                    preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                    newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
                    newErrorComp.addError(functionError);
                }
            }
            return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};

// remquo casts
template<typename N, typename E, typename P, typename arithmeticTYPE(T)>
inline auto remquo(const S<N,E,P>& n1, const T& n2, int* quot) -> SreturnType(n1.number,n2)
{
    return remquo(SreturnType(n1.number,n2)(n1), SreturnType(n1.number,n2)(n2), quot);
};
template<typename N, typename E, typename P, typename arithmeticTYPE(T)>
inline auto remquo(const T& n1, const S<N,E,P>& n2, int* quot) -> SreturnType(n1, n2.number)
{
    return remquo(SreturnType(n1,n2.number)(n1), SreturnType(n1,n2.number)(n2), quot);
};
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2>
inline auto remquo(const S<N1,E1,P1>& n1, const S<N2,E2,P2>& n2, int* quot) -> SreturnType(n1.number, n2.number)
{
    return remquo(SreturnType(n1.number,n2.number)(n1), SreturnType(n1.number,n2.number)(n2), quot);
};

// ---------- FLOATING POINT MANIPULATION FUNCTIONS ----------

// copysign
templated inline const Snum copysign(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n2, Snum(typename Snum::NumberType(0.)));

    numberType newNumber = std::copysign(n1.number, n2.number);
    if (std::signbit(newNumber) == std::signbit(n1.number))
    {
        return n1;
    }
    else
    {
        return -n1;
    }
};
set_Sfunction2_casts(copysign);

// nan
templated const Snum nan(const char* tagp)
{
    return Snum(std::nan(tagp));
};

// nextafter
templated inline const Snum nextafter(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);

    numberType result = std::nextafter(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::nextafter(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
        if((n1.error == 0.) && (n2.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if (n2.error == 0.)
        {
            preciseType proportionalInput1Error = totalError / n1.error;
            newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
        }
        else if (n1.error == 0.)
        {
            preciseType proportionalInput2Error = totalError / n2.error;
            newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
        }
        else
        {
            preciseType preciseCorrectedBut1Result = std::nextafter((preciseType)n1, n2.corrected_number());
            preciseType preciseCorrectedBut2Result = std::nextafter(n1.corrected_number(), (preciseType)n2);
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType inputError = input1Error + input2Error;
            if (inputError == 0.)
            {
                newErrorComp = Serror();
            }
            else
            {
                preciseType proportionality = totalError / inputError;
                preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
            }
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(nextafter);

// nexttoward
templated inline const Snum nexttoward(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);

    numberType result = std::nexttoward(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::nexttoward(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
        if((n1.error == 0.) && (n2.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if (n2.error == 0.)
        {
            preciseType proportionalInput1Error = totalError / n1.error;
            newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
        }
        else if (n1.error == 0.)
        {
            preciseType proportionalInput2Error = totalError / n2.error;
            newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
        }
        else
        {
            preciseType preciseCorrectedBut1Result = std::nexttoward((preciseType)n1, n2.corrected_number());
            preciseType preciseCorrectedBut2Result = std::nexttoward(n1.corrected_number(), (preciseType)n2);
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType inputError = input1Error + input2Error;
            if (inputError == 0.)
            {
                newErrorComp = Serror();
            }
            else
            {
                preciseType proportionality = totalError / inputError;
                preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
            }
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(nexttoward);

// ---------- MINIMUM MAXIMUM DIFFERENCE FUNCTIONS ----------

// fdim
templated inline const Snum fdim(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);

    numberType result = std::fdim(n1.number, n2.number);
    preciseType preciseCorrectedResult = std::fdim(n1.corrected_number(), n2.corrected_number());
    preciseType totalError = preciseCorrectedResult - result;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp;
        preciseType preciseResult = std::fdim((preciseType)n1.number, (preciseType)n2.number);
        preciseType functionError = preciseResult - result;
        if((n1.error == 0.) && (n2.error == 0.))
        {
            newErrorComp = Serror(totalError);
        }
        else if (n2.error == 0.)
        {
            preciseType proportionalInput1Error = (totalError - functionError) / n1.error;
            newErrorComp = Serror(n1.errorComposants, [proportionalInput1Error](errorType e){return e*proportionalInput1Error;});
            newErrorComp.addError(functionError);
        }
        else if (n1.error == 0.)
        {
            preciseType proportionalInput2Error = (totalError - functionError) / n2.error;
            newErrorComp = Serror(n2.errorComposants, [proportionalInput2Error](errorType e){return e*proportionalInput2Error;});
            newErrorComp.addError(functionError);
        }
        else
        {
            preciseType preciseCorrectedBut1Result = std::fdim((preciseType)n1, n2.corrected_number());
            preciseType preciseCorrectedBut2Result = std::fdim(n1.corrected_number(), (preciseType)n2);
            preciseType input1Error = preciseCorrectedResult - preciseCorrectedBut1Result;
            preciseType input2Error = preciseCorrectedResult - preciseCorrectedBut2Result;
            preciseType inputError = input1Error + input2Error;
            if (inputError == 0.)
            {
                // avoid division by zero
                newErrorComp = Serror(functionError);
            }
            else
            {
                preciseType proportionality = (totalError - functionError) / inputError;
                preciseType proportionalInput1Error = proportionality * (input1Error / n1.error);
                preciseType proportionalInput2Error = proportionality * (input2Error / n2.error);
                newErrorComp = Serror(n1.errorComposants, n2.errorComposants, [proportionalInput1Error, proportionalInput2Error](errorType e1, errorType e2){return e1*proportionalInput1Error + e2*proportionalInput2Error;});
                newErrorComp.addError(functionError);
            }
        }
        return Snum(result, totalError, newErrorComp);
    #else
        return Snum(result, totalError);
    #endif
};
set_Sfunction2_casts(fdim);

// min
templated inline const Snum min(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return std::min(n1,n2);
};
set_Sfunction2_casts(min);

// max
templated inline const Snum max(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return std::max(n1,n2);
};
set_Sfunction2_casts(max);

// fmin
templated inline const Snum fmin(const Snum& n1, const Snum& n2)
{
    return Sstd::min(n1, n2);
};
set_Sfunction2_casts(fmin);

// fmax
templated inline const Snum fmax(const Snum& n1, const Snum& n2)
{
    return Sstd::max(n1, n2);
};
set_Sfunction2_casts(fmax);

// ---------- OTHER FUNCTIONS ----------

// abs
templated inline const Snum abs(const Snum& n)
{
    Snum::checkUnstableBranch(n, Snum(typename Snum::NumberType(0.)));

    if (std::signbit(n.number)) // <=> n.number < 0 (but safe for nan)
    {
        return -n;
    }
    else
    {
        return n;
    }
};

// fabs
templated inline const Snum fabs(const Snum& n)
{
    return abs(n);
};

// fma
templated const Snum fma(const Snum& n1, const Snum& n2, const Snum& n3)
{
    numberType result = std::fma(n1.number, n2.number, n3.number);

    numberType remainder = EFT::ErrorFma(n1.number, n2.number, n3.number, result);
    //errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error) + n3.error;
    errorType newError = std::fma(n2.number, n1.error, std::fma(n1.number, n2.error, remainder + n3.error));

    #ifdef SHAMAN_TAGGED_ERROR
        numberType number1 = n1.number;
        numberType number2 = n2.number;
        Serror newErrorComp(n1.errorComposants, n2.errorComposants, [number1, number2](errorType e1, errorType e2){return number1*e2 + number2*e1;});
        newErrorComp.addErrors(n3.errorComposants);
        newErrorComp.addError(remainder);
        return Snum(result, newError, newErrorComp);
    #else
        return Snum(result, newError);
    #endif
};
set_Sfunction3_casts(fma);

// ---------- CLASSIFICATION FUNCTIONS ----------

// fpclassify
templated inline const int fpclassify(const Snum &x)
{
    return std::fpclassify(x.number);
};

// isfinite
templated inline bool isfinite(const Snum &n)
{
    return std::isfinite(n.number);
};

// isinf
templated inline bool isinf(const Snum &n)
{
    return std::isinf(n.number);
};

// isnan
templated inline bool isnan(const Snum &n)
{
    return std::isnan(n.number);
};

// isnormal
templated inline bool isnormal(const Snum &n)
{
    // does not include unstability test to check 0 and subnormal
    return std::isnormal(n.number);
};

// signbit
templated inline bool signbit(const Snum &n)
{
    Snum::checkUnstableBranch(n, Snum(typename Snum::NumberType(0.)));
    return std::signbit(n.number);
};

// ---------- COMPARISON FUNCTIONS ----------

// isgreater
templated inline const bool isgreater(const Snum &n1, const Snum &n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return std::isgreater(n1.number, n2.number);
};

// isgreaterequal
templated inline const bool isgreaterequal(const Snum &n1, const Snum &n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return std::isgreaterequal(n1.number, n2.number);
};

// isless
templated inline const bool isless(const Snum &n1, const Snum &n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return std::isless(n1.number, n2.number);
};

// islessequal
templated inline const bool islessequal(const Snum &n1, const Snum &n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return std::islessequal(n1.number, n2.number);
};

// islessgreater
templated inline const bool islessgreater(const Snum &n1, const Snum &n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return std::islessgreater(n1.number, n2.number);
};

// isunordered
templated inline const bool isunordered(const Snum &n1, const Snum &n2)
{
    return std::isunordered(n1.number, n2.number);
};

// ---------- STRING OPERATIONS ----------

/*
 * function to convert a Snum into a string
 *
 * NOTE : code duplicated from .to_string()
 */
templated inline std::string to_string(const Snum& n)
{
    return n.to_string();
};

}
#endif

using Sstd::cos;
using Sstd::sin;
using Sstd::tan;
using Sstd::atan;
using Sstd::acos;
using Sstd::asin;
using Sstd::atan2;
using Sstd::cosh;
using Sstd::sinh;
using Sstd::tanh;
using Sstd::asinh;
using Sstd::acosh;
using Sstd::atanh;
using Sstd::exp;
using Sstd::exp2;
using Sstd::expm1;
using Sstd::ilogb;
using Sstd::frexp;
using Sstd::ldexp;
using Sstd::log;
using Sstd::log10;
using Sstd::modf;
using Sstd::log1p;
using Sstd::log2;
using Sstd::logb;
using Sstd::scalbn;
using Sstd::scalbln;
using Sstd::cbrt;
using Sstd::pow;
using Sstd::sqrt;
using Sstd::hypot;
using Sstd::erf;
using Sstd::erff;
using Sstd::erfl;
using Sstd::erfc;
using Sstd::erfcf;
using Sstd::erfcl;
using Sstd::tgamma;
using Sstd::lgamma;
using Sstd::ceil;
using Sstd::floor;
using Sstd::trunc;
using Sstd::round;
using Sstd::rint;
using Sstd::nearbyint;
using Sstd::fmod;
using Sstd::lround;
using Sstd::llround;
using Sstd::lrint;
using Sstd::llrint;
using Sstd::remainder;
using Sstd::remquo;
using Sstd::copysign;
using Sstd::nan;
using Sstd::nextafter;
using Sstd::nexttoward;
using Sstd::fdim;
using Sstd::min;
using Sstd::max;
using Sstd::fmin;
using Sstd::fmax;
using Sstd::abs;
using Sstd::fabs;
using Sstd::fma;
using Sstd::fpclassify;
using Sstd::isfinite;
using Sstd::isinf;
using Sstd::isnan;
using Sstd::isnormal;
using Sstd::signbit;
using Sstd::isgreater;
using Sstd::isgreaterequal;
using Sstd::isless;
using Sstd::islessequal;
using Sstd::islessgreater;
using Sstd::isunordered;

#undef set_Sfunction2_casts
#undef set_Sfunction3_casts
