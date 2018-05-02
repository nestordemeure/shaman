//
// Created by demeuren on 02/05/18.
//

#ifndef SHAMAN_OPERATORS_H
#define SHAMAN_OPERATORS_H

#include "../Shaman.h"

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
#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = n.isNumericalZero;
#endif

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = n.doubtLevel;
#endif

    return Snum(-n.number, -n.error ISNUMERICALZERO DOUBTLEVEL);
};

// +
templated inline const Snum operator+(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number + n2.number;

    numberType remainder = EFT::eft2Sum(n1.number, n2.number, result);
    errorType newError = remainder + (n1.error + n2.error);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
#endif

    CANCELATION_TEST(Snum::minPrecision(n1,n2));

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n1.non_significativ() || n2.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Soperator_casts(+);

// -
templated inline const Snum operator-(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number - n2.number;

    numberType remainder = EFT::eft2Sum(n1.number, -n2.number, result);
    errorType newError = remainder + (n1.error - n2.error);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
#endif

    CANCELATION_TEST(Snum::minPrecision(n1,n2));

#ifdef NUMERICAL_ZERO_FIELD_ENABLED
    bool isNumericalZero = Snum::non_significativ(result,newError);
#endif

    NUMERICAL_ZERO_TEST(n1.non_significativ() || n2.non_significativ());

    return Snum(result, newError ISNUMERICALZERO DOUBTLEVEL);
};
set_Soperator_casts(-);

// *
templated inline const Snum operator*(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number * n2.number;

    numberType remainder = EFT::eftFast2Mult(n1.number, n2.number, result);
    //errorType newError = remainder + (n1.number*n2.error + n2.number*n1.error);
    errorType newError = std::fma(n1.number, n2.error, std::fma(n2.number, n1.error, remainder));
    // TODO alternative formula with a small additional term (ignored by rump but useful when n1*n2==0 while n1!=0 and n2!=0)
    //errorType newError = std::fma(n1.error, n2.error, std::fma(n1.number, n2.error, std::fma(n2.number, n1.error, remainder)));

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
    if (n1.non_significativ() && n2.non_significativ())
    {
#ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableMultiplications++;
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
set_Soperator_casts(*);

// /
templated inline const Snum operator/(const Snum& n1, const Snum& n2)
{
    numberType result = n1.number / n2.number;

    numberType remainder = - std::fma(n2.number, result, -n1.number);
    //errorType newError = ((remainder + n1.error) - result*n2.error) / (n2.number + n2.error);
    errorType newError = - std::fma(result, n2.error, -(remainder + n1.error)) / (n2.number + n2.error);

#ifdef DOUBT_LEVEL_FIELD_ENABLED
    int doubtLevel = std::max(n1.doubtLevel, n2.doubtLevel);
    if (n2.non_significativ())
    {
#ifdef UNSTABLE_OP_DEBUGGER
        NumericalDebugger::unstableMultiplications++;
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
set_Soperator_casts(/);

//-----------------------------------------------------------------------------
// CLASS OPERATORS

// ++
templated inline Snum& Snum::operator++(int)
{
    *this = (*this) + 1;

    return *this;
}

// --
templated inline Snum& Snum::operator--(int)
{
    *this = (*this) - 1;

    return *this;
}

// +=
templated inline Snum& Snum::operator+=(const Snum& n)
{
    *this = (*this) + n;

    return *this;
}

// -=
templated inline Snum& Snum::operator-=(const Snum& n)
{
    *this = (*this) - n;

    return *this;
}

// *=
templated inline Snum& Snum::operator*=(const Snum& n)
{
    *this = (*this) * n;

    return *this;
}

// /=
templated inline Snum& Snum::operator/=(const Snum& n)
{
    *this = (*this) / n;

    return *this;
}

//-----------------------------------------------------------------------------
// BOOLEAN OPERATORS

// ==
templated inline bool operator==(const Snum& n1, const Snum& n2)
{
    UNSTABLE_BRANCH_TEST;
    return n1.number == n2.number;
};
set_Sbool_operator_casts(==);

// !=
templated inline bool operator!=(const Snum& n1, const Snum& n2)
{
    UNSTABLE_BRANCH_TEST;
    return n1.number != n2.number;
};
set_Sbool_operator_casts(!=);

// <
templated inline bool operator<(const Snum& n1, const Snum& n2)
{
    UNSTABLE_BRANCH_TEST;
    return n1.number < n2.number;
};
set_Sbool_operator_casts(<);

// <=
templated inline bool operator<=(const Snum& n1, const Snum& n2)
{
    UNSTABLE_BRANCH_TEST;
    return n1.number <= n2.number;
};
set_Sbool_operator_casts(<=);

// >
templated inline bool operator>(const Snum& n1, const Snum& n2)
{
    UNSTABLE_BRANCH_TEST;
    return n1.number > n2.number;
};
set_Sbool_operator_casts(>);

// >=
templated inline bool operator>=(const Snum& n1, const Snum& n2)
{
    UNSTABLE_BRANCH_TEST;
    return n1.number >= n2.number;
};
set_Sbool_operator_casts(>=);

#endif //SHAMAN_OPERATORS_H