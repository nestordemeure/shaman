//
// Created by demeuren on 26/01/18.
//

#ifndef COMPENSATIONS_TESTS_H
#define COMPENSATIONS_TESTS_H

#include <iostream>
#include <cadna.h>
#include "../shaman/shaman/Shaman.h"

using Cdouble = double_st;
using real = Sdouble;

//---------------------------------------------------------------------------------------
// FUNCTIONS

/*
 * displays the absolute difference between a computed value and the expected value
 */
templated inline void displayError(Snum result, double expectedResult)
{
    double error = result.number - expectedResult;
    double digits = -log10(std::abs(error / result.number));
    double estimatedDigits = Snum::digits(result);

    std::cout << "result=" << result.number
              << std::endl
              << " absolut_error=" << error
              << " estimated_absolut_error=" << result.error
              << std::endl
              << " significativ_digits=" << digits
              << " estimated_significativ_digits=" << estimatedDigits
              << '\n' << std::endl;
}

/*
 * displays the absolute difference between a computed value and the expected value
 */
inline void displayError(double result, double expectedResult)
{
    double error = result - expectedResult;
    double digits = -log10(std::abs(error / result));

    std::cout << "result=" << result
              << std::endl
              << " absolut_error=" << error
              << std::endl
              << " significativ_digits=" << digits
              << '\n' << std::endl;
}

//---------------------------------------------------------------------------------------
// TESTS

/*
 * proposed by Siegfried Rump (rump's royal pain)
 * http://www.davidhbailey.com/dhbtalks/dhb-num-repro.pdf
 *
 * the result is astronomicaly wrong !
 * but consistant across precisions
 */
inline void rumpTest()
{
    std::cout << "rump test" << std::endl;

    real x = 77617.0;
    real y = 33096.0;
    real x2 = x*x;
    real y2 = y*y;
    real y4 = y2*y2;
    real y6 = y4*y2;
    real y8 = y4*y4;

    real result = 333.75*y6 + x2*(11.0*x2*y2 - y6 - 121.0*y4 - 2.0) + 5.5*y8 + x/(2.0*y);

    displayError(result, -0.82739605994682136814116);
}

//-----

/*
 * from Kahan, On the Cost of Floating-Point Computation Without Extra-Precise Arithmetic
 *
 * a nice example of Verificarlo working as expected on a problem
 */
inline void polynomialTest()
{
    std::cout << "polynomial test" << std::endl;

    // a*x² + b*x + c = 0
    real a =  94906265.625;
    real b = -189812534.0;
    real c =  94906268.375;

    real delta = b*b - 4.0*a*c;
    //number r1 = (-b + sqrt(delta)) / (2*a);
    real r2 = (-b - sqrt(delta)) / (2.0*a);

    //displayError(r1, 1.000000028975958); //aproximate root
    displayError(r2, 1); //exact root
}

//-----

/*
*  Scheme proposed by Muller [23] and analyzed by Kahan
*
*  The exact solution is 6, a repulsive fixed-point
*  However there is an attractive fixed point : 100
*  Any approximate calculation will converge toward that fixed point
*
*  This problem seems undetectable with MCA (which makes sense, MCA is all about perturbations)
*/
inline void fixedPointTest()
{
    std::cout << "Fixed-point test" << std::endl;

    real x0 = real(11.0) / real(2.0);
    real x1 = real(61.0) / real(11.0);

    for(int i = 1; i <= 100; i++)
    {
        real x2 = 111.0 - (1130.0 - 3000.0/x0)/x1;
        x0 = x1;
        x1 = x2;
    }

    displayError(x1, 6);
}

//-----

inline real identity(real x)
{
    std::cout << "Identity test (x=" << x << ')' << std::endl;

    // sqrt(x) 128 times
    for(int i = 1; i <= 128; i++)
    {
        x = sqrt(x);
    }

    // x squared 128 times
    for(int i = 1; i <= 128; i++)
    {
        x = x*x;
    }

    return x;
}

/*
 * proposed by kahan
 * How Futile are Mindless Assessments of Roundoff in Floating-Point Computation ?
 *
 * the function should be the identity but instead become 'if x < 1 then 0 else 1'
 * that function is wrong (except in 0 and 1) but very stable to perturbations
 *
 * MCA fail to detect problems around 0
 * (we might detect such comportment by analysing the sensitivity to perturbations in inputs)
 */
inline void kahanIdentity()
{
    displayError(identity(0.0), 0);
    displayError(identity(0.5), 0.5);
    displayError(identity(0.75), 0.75);
    displayError(identity(1.0), 1);
    displayError(identity(2.0), 2);
    displayError(identity(0.9), 0.9);
}

//-----

inline real E(real z)
{
    if (z == 0.0)
    {
        return 1.0;
    }
    else
    {
        return (exp(z) - 1.0)/z;
    }
}

inline real Q(real x)
{
    real sq = sqrt(x*x + 1.0);
    return fabs(x - sq) - 1.0/(x + sq);
}

inline real H(real x)
{
    std::cout << "Muller test (x=" << (float) x << ')' << std::endl;

    return E(Q(Q(x)));
}

/*
 * proposed by Jean-Michel Muller
 * http://www.davidhbailey.com/dhbtalks/dhb-num-repro.pdf
 *
 * actually correct using doubles (except for 17)
 * but VERY sensible to perturbations
 * goes very wrong with MCA
 */
inline void mullerTest()
{
    displayError(H(15.0), 1);
    displayError(H(16.0), 1);
    displayError(H(17.0), 1);
    displayError(H(9999.0), 1);
}

#endif //COMPENSATIONS_TESTS_H
