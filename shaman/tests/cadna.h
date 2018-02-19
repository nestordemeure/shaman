//
// Created by demeuren on 30/01/18.
//

#ifndef COMPENSATIONS_CADNA_H
#define COMPENSATIONS_CADNA_H

#include <iostream>
#include <array>
#include "../shaman/shaman/Shaman.h"
#include "../shaman/shamanizer/tinyformat.h"
#include "tests.h"

//---------------------------------------------------------------------------------------
// POLYNOMIAL

real P(real x, real y)
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

    real x = 10864.0;
    real y = 18817.0;
    real res = P(x,y);
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

    real a = 0.3;
    real b = -2.1;
    real c = 3.675;

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
            real x1 = - c/b;
            std::cout << "The equation is degenerated. There is one real64 solution" << x1 << std::endl;
        }
    }
    else
    {
        b = b/a;
        c = c/a;
        real d = b*b - 4.0*c;
        std::cout << "d = " << d << std::endl;
        //displayError(d, 0);

        if (d == 0.0)
        {
            real x1 = -b*0.5;
            std::cout << "Discriminant is zero. The double solution is " << x1 << std::endl;
            //displayError(x1, 3.5);
        }
        else if (d > 0.0)
        {
            real x1 = ( - b - sqrt(d))*0.5;
            real x2 = ( - b + sqrt(d))*0.5;
            std::cout << "There are two real64 solutions. x1 = " << x1 << " x2 = " << x2 << std::endl;
            //displayError(x1, 3.5);
            //displayError(x2, 3.5);
        }
        else
        {
            real x1 = - b*0.5;
            real x2 = sqrt(-d)*0.5;
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

    std::array<std::array<real, 11>, 11> amat;
    for (int i = 1; i <= 11; i++)
    {
        for (int j = 1; j <= 11; j++)
        {
            amat[i-1][j-1] = 1.0 / (i+j-1);
        }
    }
    real det = 1.0;

    int i;
    for (i = 1; i <= 10; i++)
    {
        real pivot = amat[i-1][i-1];
        std::cout << "Pivot number " << i << " = " << pivot << std::endl;
        det = det * pivot;

        real aux = 1.0 / pivot;
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

    real pivot = amat[i-1][i-1];
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

    real a = 5.5;
    real b = 61.0/11.0;

    for (int i = 3; i <= 25; i++)
    {
        real c = b;
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

    real eps = 1.0e-12;
    int nmax = 100;

    int i;
    real y = 0.5;
    real x = y + 2.0*eps;
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

    xsol[0] = 1.;
    xsol[1] = 1.;
    xsol[2] = 1.e-8;
    xsol[3] = 1.;
    a[0][0] = 21.0;
    a[0][1] = 130.0;
    a[0][2] = 0.0;
    a[0][3] = 2.1;
    a[0][4] = 153.1;
    a[1][0] = 13.0;
    a[1][1] = 80.0;
    a[1][2] = 4.74e+8;
    a[1][3] = 752.0;
    a[1][4] = 849.74;
    a[2][0] = 0.0;
    a[2][1] = -0.4;
    a[2][2] = 3.9816e+8;
    a[2][3] = 4.2;
    a[2][4] = 7.7816;
    a[3][0] = 0.0;
    a[3][1] = 0.0;
    a[3][2] = 1.7;
    a[3][3] = 9.0e-9;
    a[3][4] = 2.6e-8;

    for (int i = 1; i<idim; i++)
    {
        Sfloat aux;
        int ll = 0;

        Sfloat pmax = 0.0;
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
double random1()
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

    Sfloat eps = 1e-4;
    int ndim = 20;
    int niter = 1000;
    std::array<std::array<Sfloat, 20>, 20> a;
    std::array<Sfloat, 20> b;
    std::array<Sfloat, 20> x;
    std::array<Sfloat, 20> y;
    std::array<Sfloat, 20> xsol;

    xsol[0] = 1.7;
    xsol[1] = -4746.89;
    xsol[2] = 50.23;
    xsol[3] = -245.32;
    xsol[4] = 4778.29;
    xsol[5] = -75.73;
    xsol[6] = 3495.43;
    xsol[7] = 4.35;
    xsol[8] = 452.98;
    xsol[9] = -2.76;
    xsol[10] = 8239.24;
    xsol[11] = 3.46;
    xsol[12] = 1000.0;
    xsol[13] = -5.0;
    xsol[14] = 3642.4;
    xsol[15] = 735.36;
    xsol[16] = 1.7;
    xsol[17] = -2349.17;
    xsol[18] = -8247.52;
    xsol[19] = 9843.57;

    for (int i = 0; i < ndim; i++)
    {
        for (int j = 0; j < ndim; j++)
        {
            a[i][j] = random1();
        }
        a[i][i] = a[i][i] + 4.9213648;
    }
    for (int i = 0; i < ndim; i++)
    {
        Sfloat aux = 0.0;
        for (int j = 0; j < ndim; j++)
        {
            aux = aux + a[i][j] * xsol[j];
        }
        b[i] = aux;
        y[i] = 10.0;
    }

    int i;
    Sfloat anorm = 1.0 + eps;
    for (i = 0; (i < niter) && (anorm >= eps); i++)
    {
        anorm = 0.0;
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

real squaredTriangleArea(real x, real y, real z)
{
    return (x + y + z) * (z - (x - y)) * (z + (x - y)) * (x + (y - z));
}

/*
 * cadna find 0 significative digits here, a false positive (identified by kahan)
 */
void falsePositive()
{
    std::cout << "False positive." << '\n'
              << "Example of a problem where CADNA is said to falsly detects that the result has no significative digits"
              << std::endl;

    real x = 1.234568e6;
    real y = 1.234567e6;
    real z = 1.00000000023;
    real r = sqrt( squaredTriangleArea(x,y,z) / squaredTriangleArea(x,y,2.0*z) );

    std::cout << "ratio of areas : " << r << " (exact result  1.238278374e-05)" << std::endl;

    displayError(r, 1.238278374e-05);
}

#endif //COMPENSATIONS_CADNA_H
