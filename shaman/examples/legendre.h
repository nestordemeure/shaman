//
// Created by demeuren on 12/02/18.
//

#ifndef COMPENSATIONS_LEGENDRE_H
#define COMPENSATIONS_LEGENDRE_H

#include <array>
#include "../shaman/Shaman.h"

//-----------------------------------------------------------------------------
// POLYNOMIAL EVALUATION

// return the coefiscients of the 20th legendre polynomial
std::array<Sdouble, 21> legendre20 = []()
{
    std::array<Sdouble, 21> result;

    result[20] = Sdouble(34461632205) / Sdouble(262144);
    result[19] = 0.;
    result[18] = Sdouble(-83945001525) / Sdouble(131072);
    result[17] = 0.;
    result[16] = Sdouble(347123925225) / Sdouble(262144);
    result[15] = 0.;
    result[14] = Sdouble(-49589132175) / Sdouble(32768);
    result[13] = 0.;
    result[12] = Sdouble(136745788725) / Sdouble(131072);
    result[11] = 0.;
    result[10] = Sdouble(-29113619535) / Sdouble(65536);
    result[9] = 0.;
    result[8] = Sdouble(15058768725) / Sdouble(131072);
    result[7] = 0.;
    result[6] = Sdouble(-557732175) / Sdouble(32768);
    result[5] = 0.;
    result[4] = Sdouble(334639305) / Sdouble(262144);
    result[3] = 0.;
    result[2] = Sdouble(-4849845) / Sdouble(131072);
    result[1] = 0.;
    result[0] = Sdouble(46189) / Sdouble(262144);

    return result;
}();

//-----

// compute a a value using Horner's algorithm
Sdouble legendreHorner(Sdouble x)
{
    Sdouble result = 0.0;

    for (int i = 20; i >= 0; i--)
    {
        Sdouble coef = legendre20[i];
        result = result*x + coef;

        // TODO it might be interesting to implement this operation with an FMA to see the impact on precision
        // result = fma(result,x,coef);
    }

    return result;
}

// compute a value using the naive polynomial evaluation algorithm
Sdouble legendreNaive(Sdouble x)
{
    Sdouble result = 0.0;

    for (int i = 20; i >= 0; i--)
    {
        Sdouble coef = legendre20[i];
        result += coef * Sstd::pow(x,Sdouble(i));
    }

    return result;
}

// computes a value using the recurence formula
Sdouble legendreRec(int q, Sdouble x)
{
    if (q == 0)
    {
        return 1.0;
    }
    else if (q == 1)
    {
        return x;
    }
    else
    {
        Sdouble p0 = 1.0;
        Sdouble p1 = x;
        Sdouble pn = x;

        for (int n = 2; n <= q; n++)
        {
            //pn = (1/Sdouble(n)) * (x*(2*n-1)*p1 - (n-1)*p0);
            pn = (x * Sdouble(2*n-1) * p1 - Sdouble(n-1) * p0) / Sdouble(n);
            p0 = p1;
            p1 = pn;
        }

        return pn;
    }
}

//-----------------------------------------------------------------------------
// TESTS

/*
 * test on different evaluation methods for the legendre polynomials applied to a given x
 */
void legendre20Testx(Sdouble x)
{
    std::cout << "x = " << x << '\n'
              << "P_naive(x)\t=\t" << legendreNaive(x) << '\n'
              << "P_horner(x)\t=\t" << legendreHorner(x) << '\n'
              << "P_rec(x)\t=\t" << legendreRec(20, x) << '\n' << std::endl;
}

/*
 * test on different evaluation methods for the legendre polynomials
 * the Horner's method loses accuracy as we get closer to 1
 * meanwhile the recurcive method stays very accurate
 *
 * inspired by "some experiments with Evaluation of Legendre Polynomials"
 */
void legendre20Test()
{
    std::cout << "It has been observed that Horner's algorithm lose most of its significativ digits while computing legendre's polynomial around 1-." << '\n'
              << "Can we reproduce those observations ?" << std::endl;

    legendre20Testx(0.5);
    legendre20Testx(0.6);
    legendre20Testx(0.8);
    legendre20Testx(0.9);
    legendre20Testx(0.99);
    legendre20Testx(0.999);
}

#endif //COMPENSATIONS_LEGENDRE_H