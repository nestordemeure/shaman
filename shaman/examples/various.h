//
// Created by demeuren on 26/01/18.
//

#ifndef COMPENSATIONS_TESTS_H
#define COMPENSATIONS_TESTS_H

#include <iostream>
#include "../shaman/Shaman.h"

//---------------------------------------------------------------------------------------
// FUNCTIONS

/*
 * displays the absolute difference between a computed value and the expected value
 */
template<typename numberType, typename errorType, typename preciseType>
inline void displayError(S<numberType,errorType,preciseType> result, double expectedResult)
{
    double error = result.number - expectedResult;
    double digits = -log10(std::abs(error / result.number));
    double estimatedDigits = result.digits();

    std::cout << "result=" << result << " (" << result.number << ')'
              << '\n'
              << " absolut_error=" << error
              << " estimated_absolut_error=" << result.error
              << '\n'
              << " significativ_digits=" << digits
              << " estimated_significativ_digits=" << estimatedDigits
              << '\n' << std::endl;
}

/*
 * displays the absolute difference between a computed value and the expected value
 */
template<typename T> inline void displayError(T result, double expectedResult)
{
    double fresult = result;
    double error = fresult - expectedResult;
    double digits = -log10(std::abs(error / fresult));

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

    Sdouble x = 77617.0;
    Sdouble y = 33096.0;
    Sdouble x2 = x*x;
    Sdouble y2 = y*y;
    Sdouble y4 = y2*y2;
    Sdouble y6 = y4*y2;
    Sdouble y8 = y4*y4;

    Sdouble result = 333.75*y6 + x2*(11.0*x2*y2 - y6 - 121.0*y4 - 2.0) + 5.5*y8 + x/(2.0*y);

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
    Sdouble a =  94906265.625;
    Sdouble b = -189812534.0;
    Sdouble c =  94906268.375;

    Sdouble delta = b*b - 4.0*a*c;
    //number r1 = (-b + sqrt(delta)) / (2*a);
    Sdouble r2 = (-b - Sstd::sqrt(delta)) / (2.0*a);

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

    Sdouble x0 = Sdouble(11.0) / Sdouble(2.0);
    Sdouble x1 = Sdouble(61.0) / Sdouble(11.0);

    for(int i = 1; i <= 50; i++)
    {
        Sdouble x2 = 111.0 - (1130.0 - 3000.0/x0)/x1;

        x0 = x1;
        x1 = x2;

        std::cout << x1 << std::endl;
    }

    displayError(x1, 6);
}

//-----

inline Sdouble identity(Sdouble x)
{
    std::cout << "Identity test (x=" << x << ')' << std::endl;
    //Sdouble xinit = x;

    // sqrt(x) 128 times
    for(int i = 1; i <= 128; i++)
    {
        x = Sstd::sqrt(x);

        //Sdouble xTarget = pow(xinit,1./pow(2,i));
        //std::cout << x << std::endl;
    }

    // x squared 128 times
    for(int i = 1; i <= 128; i++)
    {
        x = x*x;

        //Sdouble xTarget = pow(xinit,1./pow(2,128-i));
        //std::cout << x << " (number=" << x.number << " errors=" << x.errors << ")" << '\t' << xTarget << std::endl;
        //std::cout << x << std::endl;
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
 *
 * SHAMAN detects problems if we use the full form of the multiplication errors : x1*e2 + x2*e1 + e1*e2
 * and not the abreviated form used by Rump (x1*e2 + x2*e1)
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

inline Sdouble E(Sdouble z)
{
    if (z == 0.0)
    {
        return 1.0;
    }
    else
    {
        return (Sstd::exp(z) - 1.0)/z;
    }
}

inline Sdouble Q(Sdouble x)
{
    Sdouble sq = Sstd::sqrt(x*x + 1.0);
    return Sstd::fabs(x - sq) - 1.0/(x + sq);
}

inline Sdouble H(Sdouble x)
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
