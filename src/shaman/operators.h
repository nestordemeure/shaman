#pragma once

#include "eft.h"

//-----------------------------------------------------------------------------
// CASTING MACROS

/*
 * this section uses templates and macro to take an operator and produce overloads for all the needed combination of types
 * while preserving implicit cast conventions
 */

// takes a value and builds an Stype around its type
inline Sfloat makeStype(float t) { return Sfloat(t); };
inline Sdouble makeStype(double t) { return Sdouble(t); };
inline Slong_double makeStype(long double t) { return Slong_double(t); };
templated inline Snum makeStype(Snum s) { return s; };

// takes two values and builds an Stype around the type C++ would use as a return type for their sum
#define SreturnType(t1,t2) decltype(makeStype(t1 + t2))

// makes sure that a template type is an arithmetic type
#define arithmeticTYPE(T) T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type

// defines overload for arithmetic operators
#define set_Soperator_casts(OPERATOR) \
template<typename N, typename E, typename P, typename arithmeticTYPE(T)> \
  inline auto operator OPERATOR (const S<N,E,P>& n1, const T& n2) -> SreturnType(n1.number,n2) \
{ \
    return SreturnType(n1.number,n2)(n1) OPERATOR SreturnType(n1.number,n2)(n2); \
} \
template<typename N, typename E, typename P, typename arithmeticTYPE(T)> \
inline auto operator OPERATOR (const T& n1, const S<N,E,P>& n2) -> SreturnType(n1,n2.number) \
{ \
    return SreturnType(n1,n2.number)(n1) OPERATOR SreturnType(n1,n2.number)(n2); \
} \
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2> \
inline auto operator OPERATOR (const S<N1,E1,P1>& n1, const S<N2,E2,P2>& n2) -> SreturnType(n1.number,n2.number) \
{ \
    return SreturnType(n1.number,n2.number)(n1) OPERATOR SreturnType(n1.number,n2.number)(n2); \
} \

// defines overload for boolean operators
#define set_Sbool_operator_casts(OPERATOR) \
template<typename N, typename E, typename P, typename arithmeticTYPE(T)> \
inline bool operator OPERATOR (const S<N,E,P>& n1, const T& n2) \
{ \
    return SreturnType(n1.number,n2)(n1) OPERATOR SreturnType(n1.number,n2)(n2); \
} \
template<typename N, typename E, typename P, typename arithmeticTYPE(T)> \
inline bool operator OPERATOR (const T& n1, const S<N,E,P>& n2) \
{ \
    return SreturnType(n1,n2.number)(n1) OPERATOR SreturnType(n1,n2.number)(n2); \
} \
template<typename N1, typename E1, typename P1, typename N2, typename E2, typename P2> \
inline bool operator OPERATOR (const S<N1,E1,P1>& n1, const S<N2,E2,P2>& n2) \
{ \
    return SreturnType(n1.number,n2.number)(n1) OPERATOR SreturnType(n1.number,n2.number)(n2); \
} \

//-----------------------------------------------------------------------------
// ARITHMETIC OPERATORS

// unary +
templated inline const Snum operator+(const Snum& n)
{
    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp(n.errorComposants);
        return Snum(n.number, n.error, newErrorComp);
    #else
        return Snum(n.number, n.error);
    #endif
};

// unary -
templated inline const Snum operator-(const Snum& n)
{
    numberType result = -n.number;
    errorType newError = -n.error;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp(n.errorComposants, [](errorType e){return -e;});
        return Snum(result, newError, newErrorComp);
    #else
        return Snum(result, newError);
    #endif
};

// +
templated inline const Snum operator+(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number + n2.number;

    numberType remainder = EFT::TwoSum(n1.number, n2.number, result);
    errorType newError = remainder + n1.error + n2.error;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp(n1.errorComposants, n2.errorComposants, std::plus<errorType>());
        newErrorComp.addError(remainder);
        return Snum(result, newError, newErrorComp);
    #else
        return Snum(result, newError);
    #endif
};
set_Soperator_casts(+);

// -
templated inline const Snum operator-(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number - n2.number;

    numberType remainder = EFT::TwoSum(n1.number, -n2.number, result);
    errorType newError = remainder + n1.error - n2.error;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp(n1.errorComposants, n2.errorComposants, std::minus<errorType>());
        newErrorComp.addError(remainder);
        return Snum(result, newError, newErrorComp);
    #else
        return Snum(result, newError);
    #endif
};
set_Soperator_casts(-);

// *
// note : we ignore second order terms
templated inline const Snum operator*(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number * n2.number;

    numberType remainder = EFT::FastTwoProd(n1.number, n2.number, result);
    errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error);

    #ifdef SHAMAN_TAGGED_ERROR
        numberType number1 = n1.number;
        numberType number2 = n2.number;
        Serror newErrorComp(n1.errorComposants, n2.errorComposants, [number1, number2](errorType e1, errorType e2){return number2*e1 + number1*e2;});
        newErrorComp.addError(remainder);
        return Snum(result, newError, newErrorComp);
    #else
        return Snum(result, newError);
    #endif
};
set_Soperator_casts(*);

// /
templated inline const Snum operator/(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number / n2.number;

    numberType remainder = EFT::RemainderDiv(n1.number, n2.number, result);
    errorType n2Precise = n2.number + n2.error;
    errorType newError = ((remainder + n1.error) - result*n2.error) / n2Precise;

    #ifdef SHAMAN_TAGGED_ERROR
        Serror newErrorComp(n1.errorComposants, n2.errorComposants, [result](errorType e1, errorType e2){return e1 - result*e2;});
        newErrorComp.addError(remainder);
        newErrorComp.divByScalar(n2Precise);
        return Snum(result, newError, newErrorComp);
    #else
        return Snum(result, newError);
    #endif
};
set_Soperator_casts(/);

//-----------------------------------------------------------------------------
// CLASS OPERATORS

// prefix ++
templated inline Snum& Snum::operator++()
{
    numberType result = number + numberType(1);
    numberType remainder = EFT::TwoSum(number, numberType(1), result);

    number = result;
    error += remainder;

    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.addError(remainder);
    #endif

    return *this;
}

// prefix --
templated inline Snum& Snum::operator--()
{
    numberType result = number - numberType(1);
    numberType remainder = EFT::TwoSum(number, numberType(-1), result);

    number = result;
    error += remainder;

    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.addError(remainder);
    #endif

    return *this;
}

// postfix ++
templated inline Snum& Snum::operator++(int)
{
    numberType result = number + numberType(1);
    numberType remainder = EFT::TwoSum(number, numberType(1), result);

    number = result;
    error += remainder;

    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.addError(remainder);
    #endif

    return *this;
}

// postfix --
templated inline Snum& Snum::operator--(int)
{
    numberType result = number - numberType(1);
    numberType remainder = EFT::TwoSum(number, numberType(-1), result);

    number = result;
    error += remainder;

    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.addError(remainder);
    #endif

    return *this;
}

// +=
templated inline Snum& Snum::operator+=(const Snum& n)
{
    numberType result = number + n.number;
    numberType remainder = EFT::TwoSum(number, n.number, result);

    number = result;
    error += remainder + n.error;

    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.addError(remainder);
        errorComposants.addErrors(n.errorComposants);
    #endif

    return *this;
}

// -=
templated inline Snum& Snum::operator-=(const Snum& n)
{
    numberType result = number - n.number;
    numberType remainder = EFT::TwoSum(number, -n.number, result);

    number = result;
    error += remainder - n.error;

    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.addError(remainder);
        errorComposants.subErrors(n.errorComposants);
    #endif

    return *this;
}

// *=
// note : we ignore second order terms
templated inline Snum& Snum::operator*=(const Snum& n)
{
    numberType result = number * n.number;
    numberType remainder = EFT::FastTwoProd(number, n.number, result);

    error = n.number*error + remainder + number*n.error;
    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.multByScalar(n.number);
        errorComposants.addError(remainder);
        errorComposants.addErrorsTimeScalar(n.errorComposants, number);
    #endif
    number = result;

    return *this;
}

// /=
templated inline Snum& Snum::operator/=(const Snum& n)
{
    numberType result = number / n.number;
    numberType remainder = EFT::RemainderDiv(number, n.number, result);
    errorType n2Precise = n.number + n.error;

    number = result;
    error = (error + remainder - result*n.error) / n2Precise;

    #ifdef SHAMAN_TAGGED_ERROR
        errorComposants.addError(remainder);
        errorComposants.addErrorsTimeScalar(n.errorComposants, -result);
        errorComposants.divByScalar(n2Precise);
    #endif

    return *this;
}

//-----------------------------------------------------------------------------
// BOOLEAN OPERATORS

// ==
templated inline bool operator==(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return n1.number == n2.number;
};
set_Sbool_operator_casts(==);

// !=
templated inline bool operator!=(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return n1.number != n2.number;
};
set_Sbool_operator_casts(!=);

// <
templated inline bool operator<(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return n1.number < n2.number;
};
set_Sbool_operator_casts(<);

// <=
templated inline bool operator<=(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return n1.number <= n2.number;
};
set_Sbool_operator_casts(<=);

// >
templated inline bool operator>(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return n1.number > n2.number;
};
set_Sbool_operator_casts(>);

// >=
templated inline bool operator>=(const Snum& n1, const Snum& n2)
{
    Snum::checkUnstableBranch(n1, n2);
    return n1.number >= n2.number;
};
set_Sbool_operator_casts(>=);

//-----------------------------------------------------------------------------

#undef set_Soperator_casts
#undef set_Sbool_operator_casts

