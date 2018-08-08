//
// Created by demeuren on 30/01/18.
//

#ifndef COMPENSATIONS_CADNA_H
#define COMPENSATIONS_CADNA_H

#include <iostream>
#include <array>
#include "../shaman/Shaman.h"
#include "../shaman/shamanizer/tinyformat.h"
#include "various.h"

/*
 * Examples inspired by Cadna numerical examples page :
 * http://cadna.lip6.fr/Examples_Dir/Accueil.php
 */

//---------------------------------------------------------------------------------------
// POLYNOMIAL

number P(number x, number y)
{
    return 9.0*x*x*x*x - y*y*y*y + 2.0*y*y;
}

/*
 * a polynomial proposed by RUMP
 *
 * the first result is bad, the second is perfect
 * can we differentiate them ?
 *
 * One can see the difference of accuracy between the two results even if they were provided with the same algorithm.
 * It points out the importance of the data influence on the numerical quality of results given by an algorithm.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex1.php
 */
void polynomial()
{
    std::cout << "Rump equation." << '\n'
              << "Can we differentiate a bad result from a good one ?" << std::endl;

    number x = 10864.0;
    number y = 18817.0;
    number res = P(x,y);
    std::cout << "P(10864,18817) = " << res << " (exact value : 1)" << std::endl;
    // displayError(res, 1);

    x = 1.0 / 3.0;
    y = 2.0 / 3.0;
    res = P(x,y);
    std::cout << "P(1/3,2/3) = " << res << std::endl;
    //displayError(P(x,y), 0.8024691358024691); // best possible according to CADNA

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------
// SECOND ORDER EQUATION

/*
 * The standard floating point arithmetic cannot detect that d=0. The wrong branching is performed and the result is false.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex2.php
 */
void secondOrder()
{
    std::cout << "Second order equation." << '\n'
              << "Discriminant d=0, x1=x2=3.5" << std::endl;

    number a = 0.3;
    number b = -2.1;
    number c = 3.675;

    if (a == 0.0)
    {
        if (b == 0.0)
        {
            if (c == 0.0)
            {
                std::cout << "Every complex value is solution." << std::endl;
            }
            else
            {
                std::cout << "There is no solution" << std::endl;
            }
        }
        else
        {
            number x1 = - c/b;
            std::cout << "The equation is degenerated. There is one real64 solution" << x1 << std::endl;
        }
    }
    else
    {
        b = b/a;
        c = c/a;
        number d = b*b - 4.0*c;
        std::cout << "d = " << d << std::endl;
        //displayError(d, 0);

        if (d == 0.0)
        {
            number x1 = -b*0.5;
            std::cout << "Discriminant is zero. The double solution is " << x1 << std::endl;
            //displayError(x1, 3.5);
        }
        else if (d > 0.0)
        {
            number x1 = ( - b - sqrt(d))*0.5;
            number x2 = ( - b + sqrt(d))*0.5;
            std::cout << "There are two real64 solutions. x1 = " << x1 << " x2 = " << x2 << std::endl;
            //displayError(x1, 3.5);
            //displayError(x2, 3.5);
        }
        else
        {
            number x1 = - b*0.5;
            number x2 = sqrt(-d)*0.5;
            std::cout << "There are two complex solutions."
                      << "z1 = " << x1 << " + i * " << x2 << ' '
                      << "z2 = " << x1 << " + i * " << -x2
                      << std::endl;
            //displayError(x1, 3.5);
        }
    }

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------
// DETERMINANT OF A HILBERT MATRIX

/*
 * determinant of Hilbert's matrix
 *
 * The gradual loss of accuracy is pointed out by CADNA.
 * One can see that the value of the determinant is significant even if it is very "small".
 * This shows how difficult it is to judge the numerical quality of a computed result by its magnitude.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex3.php
 */
void hilbert()
{
    std::cout << "The determinant of Hilbert's matrix" << '\n'
              << "The gradual loss of accuracy is pointed out." << '\n'
              << "One can see that the value of the determinant is significant even if it is very \"small\"." << std::endl;

    std::array<std::array<number, 11>, 11> amat;
    for (int i = 1; i <= 11; i++)
    {
        for (int j = 1; j <= 11; j++)
        {
            amat[i-1][j-1] = 1.0 / (i+j-1);
        }
    }
    number det = 1.0;

    int i;
    for (i = 1; i <= 10; i++)
    {
        number pivot = amat[i-1][i-1];
        std::cout << "Pivot number " << i << " = " << pivot << std::endl;
        det = det * pivot;

        number aux = 1.0 / pivot;
        for (int j = i+1; j <= 11; j++)
        {
            amat[i-1][j-1] = amat[i-1][j-1] * aux;
        }

        for (int j = i+1; j <= 11; j++)
        {
            aux = amat[j-1][i-1];
            for (int k = i+1; k<= 11; k++)
            {
                amat[j-1][k-1] = amat[j-1][k-1] - aux*amat[i-1][k-1];
            }
        }
    }

    number pivot = amat[i-1][i-1];
    det = det * pivot;
    std::cout << "Pivot number " << i << " = " << pivot << std::endl;
    std::cout << "Determinant = " << det << std::endl;

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------
// J.M. Muller sequence

/*
 * J.M. Muller sequence
 *
 * The UNSTABLE DIVISION(S) are generated by divisions where the denominator has no significant value or is a mathematical zero.
 * Such operations make the computed trajectory turn off the exact trajectory and then, the estimation of accuracy is not possible any more.
 * Even using the double precision, the computer cannot give any significant result after the iteration number 15.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex4.php
 */
void muller()
{
    std::cout << "Muller sequence." << '\n'
              << "The unstable division make us unable to track the precision past them."
              << std::endl;

    number a = 5.5;
    number b = 61.0/11.0;

    for (int i = 3; i <= 25; i++)
    {
        number c = b;
        b = 111.0 - 1130.0/b + 3000.0/(a*b);
        a = c;
        std::cout << "U(" << i << ") = " << b << std::endl;
    }

    std::cout << "The true limit is 6." << std::endl;
    //displayError(a, 6);

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------
// Root of a polynomial by Newton's method

/*
 * Root of a polynomial by Newton's method
 *
 * With CADNA, one can see that 7 significant digits were lost (despite the apparent stability).
 * By using the symbolic debugger, one can see that, at the last iteration, the denominator is a no significant value (a computed zero) and that the last answer of the stopping criterion is not reliable.
 * CADNA allows to stop the algorithm when the subtraction x-y is no significant (there is no more information to compute at the next iteration).
 * In Newton's method, a division by a computed zero may suggest a double root. One can simplify the fraction.
 * When these two transformations are done, the code is stabilized and the results are obtained with the best accuraccy of the computer. The exact value of the root is X_sol = 3/7 = 0.428571428571428571....
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex5.php
 */
void newton()
{
    std::cout << "Root of a polynomial by Newton's method" << '\n'
              << "One can see that 7 significant digits were lost (despite the apparent stability)." << std::endl;

    number eps = 1.0e-12;
    int nmax = 100;

    int i;
    number y = 0.5;
    number x = y + 2.0*eps;
    for (i = 1; (i <= nmax) && (abs(x-y) >= eps); i++)
    {
        x = y;
        y = x - (1.47*pow(x,3.0) + 1.19*pow(x,2.0) - 1.83*x + 0.45) / (4.41*pow(x,2.0) + 2.38*x - 1.83);
    }

    std::cout << "x(" << i-1 << ") = " << x << std::endl;
    std::cout << "x(" << i << ") = " << y << std::endl;
    //displayError(y, 0.428571428571428571);

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------
// The gaussian method

/*
 * The gaussian method
 *
 * During the reduction of the third column, A(3,3) is equal to 4864. But the exact value of A(3,3) is zero. The standard foating point arithmetic cannot detect that A(3,3) is not significant.
 * This value is chosen as pivot. That leads to erroneous results.
 * CADNA detects the no significant value of A(3,3). This value is eliminated as pivot. That leads to satisfying results.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex6.php
 */
void gauss()
{
    std::cout << "the gaussian method" << '\n'
              << "During the reduction of the third column, A(3,3) is equal to 4864. But the exact value of A(3,3) is zero. The standard foating point arithmetic cannot detect that A(3,3) is not significant. This value is chosen as pivot. That leads to erroneous results." << std::endl;

    std::array<std::array<Sfloat, 5>, 4> a;
    std::array<Sfloat, 4> xsol;
    int idim = a.size();
    int idim1 = a[0].size();

    xsol[0] = 1.f;
    xsol[1] = 1.f;
    xsol[2] = 1.e-8f;
    xsol[3] = 1.f;
    a[0][0] = 21.0f;
    a[0][1] = 130.0f;
    a[0][2] = 0.0f;
    a[0][3] = 2.1f;
    a[0][4] = 153.1f;
    a[1][0] = 13.0f;
    a[1][1] = 80.0f;
    a[1][2] = 4.74e+8f;
    a[1][3] = 752.0f;
    a[1][4] = 849.74f;
    a[2][0] = 0.0f;
    a[2][1] = -0.4f;
    a[2][2] = 3.9816e+8f;
    a[2][3] = 4.2f;
    a[2][4] = 7.7816f;
    a[3][0] = 0.0f;
    a[3][1] = 0.0f;
    a[3][2] = 1.7f;
    a[3][3] = 9.0e-9f;
    a[3][4] = 2.6e-8f;

    for (int i = 1; i<idim; i++)
    {
        Sfloat aux;
        int ll = 0;

        Sfloat pmax = 0.0f;
        for (int j = i; j <= idim; j++)
        {
            if (abs(a[j-1][i-1]) > pmax)
            {
                pmax = abs(a[j-1][i-1]);
                ll = j;
            }
        }

        if (ll != i)
        {
            for (int j = i; j <= idim1; j++)
            {
                aux = a[i-1][j-1];
                a[i-1][j-1] = a[ll-1][j-1];
                a[ll-1][j-1] = aux;
            }
        }

        aux = a[i-1][i-1];
        for (int j = i+1; j <= idim1; j++)
        {
            a[i-1][j-1] = a[i-1][j-1] / aux;
        }

        for (int k = i+1; k<=idim; k++)
        {
            aux = a[k-1][i-1];
            for (int j = i+1; j<=idim1; j++)
            {
                a[k-1][j-1] = a[k-1][j-1] - aux*a[i-1][j-1];
            }
        }
    }

    a[idim-1][idim1-1] = a[idim-1][idim1-1] / a[idim-1][idim-1];

    for (int i = idim-1; i >= 1; i--)
    {
        for (int j = i+1; j <= idim; j++)
        {
            a[i-1][idim1-1] = a[i-1][idim1-1] - a[i-1][j-1] * a[j-1][idim1-1];
        }
    }

    for (int i = 1; i<= idim; i++)
    {
        std::cout << "x_sol(" << i << ") = " << a[i-1][idim1-1] << " (true value : " << (double) xsol[i-1] << ')' << std::endl;
        //displayError(a[i-1][idim1-1], (double) xsol[i-1]);
    }

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------
// Jacobi's method

int nrand = 23;
float random1()
{
    nrand = (nrand*5363 + 143) % 1387;
    return 2.0*nrand/1387.0 - 1.0;
}

/*
 * Jacobi's method
 *
 * With the classical arithmetic, the stopping criterion is satisfied at the 1000-th iteration. But, from the 30th iteration there is no numerical improvement of the results.
 * A kind of "trampling" appears. the value of eps is too small.
 * With CADNA, the program is stopped when, for all components, difference between two iterations becomes no significant. It happens at the 30-th iteration.
 * For a same numerical quality, CADNA has divided the number of iteration by a factor 30.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex7.php
 */
void jacobi()
{
    std::cout << "Jacobi's method" << std::endl;

    Sfloat eps = 1e-4f;
    int ndim = 20;
    int niter = 1000;
    std::array<std::array<Sfloat, 20>, 20> a;
    std::array<Sfloat, 20> b;
    std::array<Sfloat, 20> x;
    std::array<Sfloat, 20> y;
    std::array<Sfloat, 20> xsol;

    xsol[0] = 1.7f;
    xsol[1] = -4746.89f;
    xsol[2] = 50.23f;
    xsol[3] = -245.32f;
    xsol[4] = 4778.29f;
    xsol[5] = -75.73f;
    xsol[6] = 3495.43f;
    xsol[7] = 4.35f;
    xsol[8] = 452.98f;
    xsol[9] = -2.76f;
    xsol[10] = 8239.24f;
    xsol[11] = 3.46f;
    xsol[12] = 1000.0f;
    xsol[13] = -5.0f;
    xsol[14] = 3642.4f;
    xsol[15] = 735.36f;
    xsol[16] = 1.7f;
    xsol[17] = -2349.17f;
    xsol[18] = -8247.52f;
    xsol[19] = 9843.57f;

    for (int i = 0; i < ndim; i++)
    {
        for (int j = 0; j < ndim; j++)
        {
            a[i][j] = random1();
        }
        a[i][i] = a[i][i] + 4.9213648f;
    }
    for (int i = 0; i < ndim; i++)
    {
        Sfloat aux = 0.0f;
        for (int j = 0; j < ndim; j++)
        {
            aux = aux + a[i][j] * xsol[j];
        }
        b[i] = aux;
        y[i] = 10.0f;
    }

    int i;
    Sfloat anorm = 1.0f + eps;
    for (i = 0; (i < niter) && (anorm >= eps); i++)
    {
        anorm = 0.0f;
        for (int j = 0; j < ndim; j++)
        {
            x[j] = y[j];
        }
        for (int j = 0; j < ndim; j++)
        {
            Sfloat aux = b[j];
            for (int k = 0; k < ndim; k++)
            {
                if (k != j)
                {
                    aux = aux - a[j][k] * x[k];
                }
            }
            y[j] = aux / a[j][j];
            if (abs(x[j] - y[j]) > anorm)
            {
                anorm = abs(x[j] - y[j]);
            }
        }
    }

    std::cout << "niter = " << i << std::endl;
    for (int i = 0; i < ndim; i++)
    {
        Sfloat aux = -b[i];
        for (int j = 0; j < ndim; j++)
        {
            aux = aux + a[i][j] * y[j];
        }
        std::cout << "x_sol(" << i+1 << ") = " << y[i] << " (true value : " << (double) xsol[i] << "), residue(" << i+1 << ") = " << aux << std::endl;
        //displayError(y[i], (double) xsol[i]);
    }

    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------
// CADNA FAILURES

number squaredTriangleArea(number x, number y, number z)
{
    return (x + y + z) * (z - (x - y)) * (z + (x - y)) * (x + (y - z));
}

/*
 * cadna find 0 significative digits here, a false positive (identified by kahan)
 *
 * it seems that it was due to a bug in cadna since corrected
 */
void falsePositive()
{
    std::cout << "False positive." << '\n'
              << "Example of a problem where CADNA is said to falsly detects that the result has no significative digits"
              << std::endl;

    number x = 1.234568e6;
    number y = 1.234567e6;
    number z = 1.00000000023;
    number r = sqrt( squaredTriangleArea(x,y,z) / squaredTriangleArea(x,y,2.0*z) );

    std::cout << "ratio of areas : " << r << " (exact result  1.238278374e-05)" << std::endl;

    displayError(r, 1.238278374e-05);
}

//-----

/*
 * an example where cadna overestimate the number of significative digits
 */
void alternatingCounter()
{
    std::cout << "An example founc in verificarlo's main schrodinger." << '\n'
              << "CADNA overestimate the number of significative digits" << std::endl;

    Sdouble c = -5e13;

    for(unsigned int i = 0; i < 100000000; i++)
    {
        if (i%2 == 0)
        {
            c = c + 1.e6;
        }
        else
        {
            c = c - 1.e-6;
        }
    }

    std::cout << "result=" << c << " expected result=" << -50 << std::endl;
    displayError(c, -50);
}

//-----

/*
 * demonstration of error followed through a trigonometric function
 *
 * useful to showcase src's ability to follow error in transcendentalfunctions while others (Verrou, Cadna?, verificarlo?) just ignore them
 */
inline void trigoTest()
{
    std::cout << "An example, using a trigonometric function, where CADNA overestimate the number of significative digits." << std::endl;

    /*
     * 2e20 : no significativ digits, everybody is correct
     * 2e25 : there is one significative digit (pure luck),  src might detect it
     * 2e30 : no significative digits, cadna is optimistic by 2 digits, src is correct
     */
    double bigNum = 2e30;
    double epsi = 10;
    double result = cos(epsi);

    // double
    {
        double x = bigNum;
        double y = x + epsi;

        double sum1 = cos(x-y);
        double sum2 = cos(x)*cos(y) + sin(x)*sin(y);
        std::cout << "DOUBLE cos(x-y) = " << sum1 << " = " << sum2 << " (true result=" << result << ')' << std::endl;
    }

    // cadna
    {
        Cdouble x = bigNum;
        Cdouble y = x + epsi;

        Cdouble sum1 = cos(x-y);
        Cdouble sum2 = cos(x)*cos(y) + sin(x)*sin(y);
        std::cout << "CADNA  cos(x-y) = " << sum1 << " = " << sum2 << " (true result=" << result << ')' << std::endl;
    }

    // src
    {
        Sdouble x = bigNum;
        Sdouble y = x + epsi;

        Sdouble sum1 = cos(x-y);
        Sdouble sum2 = cos(x)*cos(y) + sin(x)*sin(y);
        std::cout << "SHAMAN cos(x-y) = " << sum1 << " = " << sum2 << " (true result=" << result << " estimated-true-result=" << (sum1.number+sum1.error) << ')' << std::endl;
    }
}

#endif //COMPENSATIONS_CADNA_H