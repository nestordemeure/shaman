#ifndef SHAMAN_EFT_H
#define SHAMAN_EFT_H

#include <cmath>

/*
 * ERROR FREE TRANSFORM
 *
 * NOTE :
 * see the handbook of floating point arithmetic for an exact analysis
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
    inline const T eft2Sum(const T n1, const T n2, const T result)
    {
        T n22 = result - n1;
        T n11 = result - n22;
        T epsilon2 = n2 - n22;
        T epsilon1 = n1 - n11;
        T error = epsilon1 + epsilon2;
        return error;
    }

    // fast EFT for a sum
    // NOTE requires hypothesis on the inputs
    // WARNING requires rounding to nearest (see Priest)
    template<typename T>
    inline const T eftFast2Sum(const T n1, const T n2, const T result)
    {
        T n22 = result - n1;
        T error = n2 - n22;
        return error;
    }

    // EFT for a sum
    // NOTE does not require rounding to nearest
    // NOTE see also "Error-Free Transformation in Rounding Mode toward Zero" for an algorithm faster for rounding toward zero
    template<typename T>
    inline const T eftPriest2Sum(const T n1, const T n2, const T result)
    {
        if (std::abs(n1) < std::abs(n2))
        {
            T temp = n1;
            n1 = n2;
            n1 = temp;
        }

        T e = result - n1;
        T g = result - e;
        T h = g - n1;
        T f = n2 - h;
        T d = f - e;

        if ((d + e) != f)
        {
            //result = n1;
            d = n2;
        }

        return d;
    }

    // fast EFT for a multiplication
    // see also dekker's multiplication algorithm (rounding to nearest) when an FMA is unavailable
    // NOTE proof for rounding toward zero in "Error-Free Transformation in Rounding Mode toward Zero"
    // WARNING proved only for rounding to nearest and toward zero
    template<typename T>
    inline const T eftFast2Mult(const T n1, const T n2, const T result)
    {
        T error = std::fma(n1, n2, -result);
        return error;
    }

    // EFT for an FMA
    // NOTE cf "Some Functions Computable with a Fused-mac" (handbook of floating point computations)
    template<typename T>
    inline const T eftErrFma(const T n1, const T n2, const T n3, const T result)
    {
        T u1 = n1 * n2;
        T u2 = eftFast2Mult(n1, n2, u1);

        T alpha1 = n3 + u2;
        T alpha2 = eft2Sum(n3, u2, alpha1);

        T beta1 = u1 + alpha1;
        T beta2 = eft2Sum(u1, alpha1, beta1);

        T gamma = (beta1 - result) + beta2;
        T error1 = gamma + alpha2;
        T error2 = eftFast2Sum(gamma, alpha2, error1);

        // TODO if we sum error1 and error2 it might be better to ignore error2 (or to add it later)
        return error1 + error2;
    }
}

#endif //SHAMAN_EFT_H
