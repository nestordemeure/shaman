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
    Serror newErrors = Serror::unaryNeg(n.errors);
    numberType newNumber = -n.number;
    return Snum(newNumber, newErrors);
};

// +
templated inline const Snum operator+(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number + n2.number;
    numberType remainder = EFT::TwoSum(n1.number, n2.number, result);

    // newError = remainder + n1.error + n2.error
    Serror newError = Serror(remainder);
    newError.addErrors(n1.errors);
    newError.addErrors(n2.errors);

    return Snum(result, newError);
};
set_Soperator_casts(+);

// -
templated inline const Snum operator-(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number - n2.number;
    numberType remainder = EFT::TwoSum(n1.number, -n2.number, result);

    // newError = remainder + n1.error - n2.error
    Serror newError = Serror(remainder);
    newError.addErrors(n1.errors);
    newError.subErrors(n2.errors);

    return Snum(result, newError);
};
set_Soperator_casts(-);

// *
templated inline const Snum operator*(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number * n2.number;
    numberType remainder = EFT::FastTwoProd(n1.number, n2.number, result);

    // newError = remainder + (n1.number*n2.error + n2.number*n1.error)
    Serror newError = Serror(remainder);
    newError.addErrorsTimeScalar(n2.errors, n1.number);
    newError.addErrorsTimeScalar(n1.errors, n2.number);
    // TODO we ignore second order error terms

    return Snum(result, newError);
};
set_Soperator_casts(*);

// /
templated inline const Snum operator/(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number / n2.number;
    numberType remainder = EFT::RemainderDiv(n1.number, n2.number, result);
    errorType n2Precise = n2.number + n2.errors.totalError;

    // newError = ((remainder + n1.error) - result*n2.error) / (n2.number + n2.error)
    Serror newError = Serror(remainder);
    newError.addErrors(n1.errors);
    newError.subErrorsTimeScalar(n2.errors, result);
    newError.divByScalar(n2Precise);

    return Snum(result, newError);
};
set_Soperator_casts(/);

//-----------------------------------------------------------------------------
// CLASS OPERATORS

// ++
templated inline Snum& Snum::operator++(int)
{
    numberType result = number + 1;
    numberType remainder = EFT::TwoSum(number, 1, result);

    // newError = this.error + remainder
    errors.addError(remainder);
    number = result;

    return *this;
}

// --
templated inline Snum& Snum::operator--(int)
{
    numberType result = number - 1;
    numberType remainder = EFT::TwoSum(number, -1, result);

    // newError = this.error + remainder
    errors.addError(remainder);
    number = result;

    return *this;
}

// +=
templated inline Snum& Snum::operator+=(const Snum& n)
{
    numberType result = number + n.number;
    numberType remainder = EFT::TwoSum(number, n.number, result);

    // newError = this.error + remainder + n.error
    errors.addError(remainder);
    errors.addErrors(n.errors);

    number = result;

    return *this;
}

// -=
templated inline Snum& Snum::operator-=(const Snum& n)
{
    numberType result = number - n.number;
    numberType remainder = EFT::TwoSum(number, -n.number, result);

    // newError = this.error + remainder - n.error
    errors.addError(remainder);
    errors.subErrors(n.errors);

    number = result;

    return *this;
}

// *=
templated inline Snum& Snum::operator*=(const Snum& n)
{
    numberType result = number * n.number;
    numberType remainder = EFT::FastTwoProd(number, n.number, result);

    // newError = n.number*this.error + remainder + number*n.error
    errors.multByScalar(n.number);
    errors.addError(remainder);
    errors.addErrorsTimeScalar(n.errors, number);
    // TODO we ignore second order error terms

    number = result;

    return *this;
}

// /=
templated inline Snum& Snum::operator/=(const Snum& n)
{
    numberType result = number / n.number;
    numberType remainder = EFT::RemainderDiv(number, n.number, result);
    errorType n2Precise = n.number + n.errors.totalError;

    // newError = (this.error + remainder - result*n.error) / (n.number + n.error)
    errors.addError(remainder);
    errors.subErrorsTimeScalar(n.errors, result);
    errors.divByScalar(n2Precise);

    return *this;
}

//-----------------------------------------------------------------------------
// BOOLEAN OPERATORS

// ==
templated inline bool operator==(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    return n1.number == n2.number;
};
set_Sbool_operator_casts(==);

// !=
templated inline bool operator!=(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    return n1.number != n2.number;
};
set_Sbool_operator_casts(!=);

// <
templated inline bool operator<(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    return n1.number < n2.number;
};
set_Sbool_operator_casts(<);

// <=
templated inline bool operator<=(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    return n1.number <= n2.number;
};
set_Sbool_operator_casts(<=);

// >
templated inline bool operator>(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    return n1.number > n2.number;
};
set_Sbool_operator_casts(>);

// >=
templated inline bool operator>=(const Snum& n1, const Snum& n2)
{
    #ifdef UNSTABLE_BRANCH_DEBUGGER
    unstableBranchTest(n1, n2);
    #endif

    return n1.number >= n2.number;
};
set_Sbool_operator_casts(>=);

//-----------------------------------------------------------------------------

#undef set_Soperator_casts
#undef set_Sbool_operator_casts

#endif //SHAMAN_OPERATORS_H