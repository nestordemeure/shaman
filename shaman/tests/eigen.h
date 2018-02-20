//
// Created by demeuren on 20/02/18.
//

#ifndef SHAMAN_DEMO_EIGEN_H
#define SHAMAN_DEMO_EIGEN_H

#include <Eigen/Core>
#include <Eigen/Dense>

//-----------------------------------------------------------------------------
// USER DEFINED TYPE

namespace Eigen
{
    /*
     * adds Sdouble to eigen
     * NumTraits<double> gives us the epsilon, dummy_precision, lowest and highest functions
     */
    template<> struct NumTraits<Sdouble> : NumTraits<double>
    {
        typedef Sdouble Real;
        typedef Sdouble NonInteger;
        typedef Sdouble Nested;
        enum
        {
            IsComplex = 0,
            IsInteger = 0,
            IsSigned = 1,
            RequireInitialization = 0,
            ReadCost = 1,
            AddCost = 1,
            MulCost = 1
        };
    };
}

using Smatrix = Eigen::Matrix<Sdouble, Eigen::Dynamic, Eigen::Dynamic>;
using Svector = Eigen::Matrix<Sdouble, Eigen::Dynamic, 1>;

//-----------------------------------------------------------------------------
// DEMO

/*
 * create a kac matrix (also known as clement matrix)
 * its eigenvalues are the (2*k - n + 1)
 */
Smatrix KacMat(int size)
{
    int n = size - 1;

    Smatrix matrix = Smatrix::Zero(size,size);

    for (int k = 1; k <= n; k++)
    {
        matrix(k, k-1) = k;
        matrix(k-1, k) = n - (k - 1);
    }

    return matrix;
}

/*
 * short demo of eigen
 */
void eigenDemo()
{
    int size = 11;

    // defining the matrix
    Smatrix A = KacMat(size);
    //std::cout << "The matrix A is:\n" << A << '\n' << std::endl;

    // finding the eigenvalues
    Eigen::SelfAdjointEigenSolver<Smatrix> eigensolver(A);
    if (eigensolver.info() != Eigen::Success)
    {
        abort();
    }
    else
    {
        Svector eigenvals = eigensolver.eigenvalues();
        //std::cout << "Eigenvalues of A :\n" << eigenvals << std::endl;
        //std::cout << "Corresponding eigenvectors of A :\n" << eigensolver.eigenvectors() << std::endl;

        for(int k = 0; k < size; k++)
        {
            Sdouble computedEigenval = eigenvals(k);
            int realeigenval = 2*k - size + 1;
            //displayError(computedEigenval,realeigenval);
            std::cout << computedEigenval << " instead of " << realeigenval << std::endl;
        }
    }
}

#endif //SHAMAN_DEMO_EIGEN_H