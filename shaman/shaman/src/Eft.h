#ifndef SHAMAN_EFT_H
#define SHAMAN_EFT_H

#include <cmath>

/*
 * ERROR FREE TRANSFORM
 *
 * gives us the exact error|remainder of an arithmetic operation using :
 * - twoSum for + (might be killed by aggressive compilation)
 * - std::fma for *, /, sqrt (reliable independent of the compilations flags)
 * - fastTwoSum for shaman::fma via errorFma (rarely|never useful, might be killed by aggressive compilation)
 *
 * NOTE :
 * see the handbook of floating point arithmetic for an exact analysis
 * all of these EFT are perfectly accurate when rounding to nearest
 * some of these EFT (addition) requires a rounding to nearest but could be replaced (priest)
 * some of these EFT (FMA based EFT) are not proved for all rounding types (in particular toward infinity)
 *
 * However it was shown that these EFT stay accurate even with directed rounding (toward infinite)
 * (Numerical validation of compensated algorithms with stochastic arithmetic)
 */

namespace EFT
{
    // basic EFT for a sum
    // WARNING requires rounding to nearest (see Priest)
    template<typename T>
    inline const T TwoSum(const T n1, const T n2, const T result)
    {
        T n22 = result - n1;
        T n11 = result - n22;
        T epsilon2 = n2 - n22;
        T epsilon1 = n1 - n11;
        T error = epsilon1 + epsilon2;
        return error;
    }

    // fast EFT for a multiplication
    // see also dekker's multiplication algorithm (rounding to nearest) when an FMA is unavailable
    // NOTE proof for rounding toward zero in "Error-Free Transformation in Rounding Mode toward Zero"
    // WARNING proved only for rounding to nearest and toward zero
    template<typename T>
    inline const T FastTwoProd(const T n1, const T n2, const T result)
    {
        T error = std::fma(n1, n2, -result);
        return error;
    }

    // computes the remainder of the division
    // see Handbook of floating point arithmetic
    template<typename T>
    inline const T RemainderDiv(const T n1, const T n2, const T result)
    {
        T remainder = -std::fma(n2, result, -n1);
        return remainder;
    }

    // computes the remainder of the sqrt
    // see Handbook of floating point arithmetic
    template<typename T>
    inline const T RemainderSqrt(const T n, const T result)
    {
        T remainder = -std::fma(result, result, -n);
        return remainder;
    }

    // fast EFT for a sum
    // NOTE requires hypothesis on the inputs (n1 > n2)
    // WARNING requires rounding to nearest (see Priest)
    template<typename T>
    inline const T FastTwoSum(const T n1, const T n2, const T result)
    {
        T n22 = result - n1;
        T error = n2 - n22;
        return error;
    }

    // EFT for a sum
    // NOTE does not require rounding to nearest
    // NOTE see also "Error-Free Transformation in Rounding Mode toward Zero" for an algorithm faster for rounding toward zero
    template<typename T>
    inline const T PriestTwoSum(const T n1, const T n2, const T result)
    {
        if (std::abs(n1) < std::abs(n2))
        {
            std::swap(n1, n2);
        }

        T n22 = result - n1;
        T n11 = result - n22;
        T epsilon1 = n11 - n1;
        T n222 = n2 - epsilon1;
        T error = n222 - n22;

        if ((error + n22) != n222)
        {
            //result = n1;
            error = n2;
        }

        return error;
    }

    // EFT for an FMA
    // NOTE cf "Some Functions Computable with a Fused-mac" (handbook of floating point computations)
    template<typename T>
    inline const T ErrorFma(const T n1, const T n2, const T n3, const T result)
    {
        T u1 = n1 * n2;
        T u2 = FastTwoProd(n1, n2, u1);

        T alpha1 = n3 + u2;
        T alpha2 = TwoSum(n3, u2, alpha1);

        T beta1 = u1 + alpha1;
        T beta2 = TwoSum(u1, alpha1, beta1);

        T gamma = (beta1 - result) + beta2;
        T error1 = gamma + alpha2;
        T error2 = FastTwoSum(gamma, alpha2, error1);

        // TODO if we sum error1 and error2 it might be better to ignore error2 (or to add it later)
        return (error1 + error2);
    }
}

#endif //SHAMAN_EFT_H
