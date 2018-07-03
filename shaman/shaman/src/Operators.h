//
// Created by demeuren on 02/05/18.
//

#ifndef SHAMAN_OPERATORS_H
#define SHAMAN_OPERATORS_H

#include "../Shaman.h"
#include "Eft.h"

//-----------------------------------------------------------------------------
// CASTING MACROS

/*
 * this section uses templates and macro to take an operator and produce overloads for all the needed combination of types
 * while preserving implicit cast conventions
 */

#ifdef EXPLICIT_CASTING

// arithmetic operator (between n and S<n,e,p> only)
#define set_Soperator_casts(OPERATOR) \
template<typename n, typename e, typename p> \
inline const S<n,e,p> operator OPERATOR (const n& n1, const S<n,e,p>& n2) \
{ \
    return S<n,e,p>(n1) OPERATOR n2; \
} \
template<typename n, typename e, typename p> \
inline const S<n,e,p> operator OPERATOR (const S<n,e,p>& n1, const n& n2) \
{ \
    return n1 OPERATOR S<n,e,p>(n2); \
} \

// boolean operator (between n and S<n,e,p> only)
#define set_Sbool_operator_casts(OPERATOR) \
template<typename n, typename e, typename p> \
inline bool operator OPERATOR (const n& n1, const S<n,e,p>& n2) \
{ \
    return S<n,e,p>(n1) OPERATOR n2; \
} \
template<typename n, typename e, typename p> \
inline bool operator OPERATOR (const S<n,e,p>& n1, const n& n2) \
{ \
    return n1 OPERATOR S<n,e,p>(n2); \
} \

#else

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

#endif

//-----------------------------------------------------------------------------
// ARITHMETIC OPERATORS

// -
templated inline const Snum operator-(const Snum& n)
{
    numberType newNumber = -n.number;
    errorType newError = -n.error;
    Serror newErrors(n.errorComposants);
    newErrors.unaryNeg();

    return Snum(newNumber, newError, newErrors);
};

// +
templated inline const Snum operator+(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number + n2.number;

    numberType remainder = EFT::TwoSum(n1.number, n2.number, result);
    errorType newError = remainder + n1.error + n2.error;


    Serror newErrorComp(n1.errorComposants);
    newErrorComp.addError(remainder);
    newErrorComp.addErrors(n2.errorComposants);

    return Snum(result, newError, newErrorComp);
};
set_Soperator_casts(+);

// -
templated inline const Snum operator-(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number - n2.number;

    numberType remainder = EFT::TwoSum(n1.number, -n2.number, result);
    errorType newError = remainder + n1.error - n2.error;


    Serror newErrorComp(n1.errorComposants);
    newErrorComp.addError(remainder);
    newErrorComp.subErrors(n2.errorComposants);

    return Snum(result, newError, newErrorComp);
};
set_Soperator_casts(-);

// *
// note : we ignore second order terms
templated inline const Snum operator*(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number * n2.number;

    numberType remainder = EFT::FastTwoProd(n1.number, n2.number, result);
    errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error);
    
    Serror newErrorComp(remainder);
    newErrorComp.addErrorsTimeScalar(n2.errorComposants, n1.number);
    newErrorComp.addErrorsTimeScalar(n1.errorComposants, n2.number);

    return Snum(result, newError, newErrorComp);
};
set_Soperator_casts(*);

// /
templated inline const Snum operator/(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number / n2.number;

    numberType remainder = EFT::RemainderDiv(n1.number, n2.number, result);
    errorType n2Precise = n2.number + n2.error;
    errorType newError = ((remainder + n1.error) - result*n2.error) / (n2.number + n2.error);


    Serror newErrorComp(n1.errorComposants);
    newErrorComp.addError(remainder);
    newErrorComp.subErrorsTimeScalar(n2.errorComposants, result);
    newErrorComp.divByScalar(n2Precise);

    return Snum(result, newError, newErrorComp);
};
set_Soperator_casts(/);

//-----------------------------------------------------------------------------
// CLASS OPERATORS

// ++
templated inline Snum& Snum::operator++(int)
{
    numberType result = number + 1;
    numberType remainder = EFT::TwoSum(number, 1, result);

    error += remainder;
    errorComposants.addError(remainder);
    number = result;

    return *this;
}

// --
templated inline Snum& Snum::operator--(int)
{
    numberType result = number - 1;
    numberType remainder = EFT::TwoSum(number, -1, result);

    error += remainder;
    errorComposants.addError(remainder);
    number = result;

    return *this;
}

// +=
templated inline Snum& Snum::operator+=(const Snum& n)
{
    numberType result = number + n.number;
    numberType remainder = EFT::TwoSum(number, n.number, result);

    error += remainder + n.error;
    errorComposants.addError(remainder);
    errorComposants.addErrors(n.errorComposants);
    number = result;

    return *this;
}

// -=
templated inline Snum& Snum::operator-=(const Snum& n)
{
    numberType result = number - n.number;
    numberType remainder = EFT::TwoSum(number, -n.number, result);

    error += remainder - n.error;
    errorComposants.addError(remainder);
    errorComposants.subErrors(n.errorComposants);
    number = result;

    return *this;
}

// *=
// note : we ignore second order terms
templated inline Snum& Snum::operator*=(const Snum& n)
{
    numberType result = number * n.number;
    numberType remainder = EFT::FastTwoProd(number, n.number, result);

    error = n.number*error + remainder + number*n.error;
    errorComposants.multByScalar(n.number);
    errorComposants.addError(remainder);
    errorComposants.addErrorsTimeScalar(n.errorComposants, number);
    number = result;

    return *this;
}

// /=
templated inline Snum& Snum::operator/=(const Snum& n)
{
    numberType result = number / n.number;
    numberType remainder = EFT::RemainderDiv(number, n.number, result);
    errorType n2Precise = n.number + n.error;

    error = (error + remainder - result*n.error) / (n.number + n.error);
    errorComposants.addError(remainder);
    errorComposants.subErrorsTimeScalar(n.errorComposants, result);
    errorComposants.divByScalar(n2Precise);
    number = result;

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

#endif //SHAMAN_OPERATORS_H