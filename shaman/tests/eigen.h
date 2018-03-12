//
// Created by demeuren on 20/02/18.
//

#ifndef SHAMAN_DEMO_EIGEN_H
#define SHAMAN_DEMO_EIGEN_H

#include "../shaman/shaman/Shaman_eigen.h"

/*
 * create a kac matrix (also known as Clement matrix)
 * its eigenvalues are the (2*k - size + 1)
 */
Eigen::SMatrixXd KacMat(int size)
{
    int n = size - 1;

    Eigen::SMatrixXd matrix = Eigen::SMatrixXd::Zero(size,size);

    for (int k = 1; k <= n; k++)
    {
        matrix(k, k-1) = k;
        matrix(k-1, k) = n - (k - 1);
    }

    return matrix;
}

/*
 * short demo of eigen
 * Shaman is very pessimistic due to some unstable operations
 */
void eigenDemo()
{
    int size = 75; // 99

    // defining the matrix
    Eigen::SMatrixXd A = KacMat(size);
    //std::cout << "The matrix A is:\n" << A << '\n' << std::endl;

    // computing the eigenvalues
    Eigen::SVectorXd eigenvals = A.eigenvalues().real();
    std::sort(eigenvals.data(), eigenvals.data() + eigenvals.size());

    // displaying the result
    //std::cout << "Eigenvalues of A :\n" << eigenvals << std::endl;
    for(int k = 0; k < size; k++)
    {
        Sdouble computedEigenval = eigenvals(k);
        int realeigenval = 2*k - size + 1;

        //displayError(computedEigenval,realeigenval);
        std::cout << computedEigenval << "\t\t\t(" << std::setprecision(17) << computedEigenval.number << " instead of " << realeigenval << ')' << std::endl;
    }
}

/*
 * significativ digits lost as a function of doubt (digits : doubt) :
 * 6 : 228
 * 6 : 228
 * 13 : 237
 * 13 : 238
 * 13 : 239
 * 12 : 238
 * 12 : 238
 * ...
 * 9 : 204
 * 10 : 185
 */

/*
 * TODO :
 * - do a small test such as a matrix multiplication (should avoid unstable operations found in eigenvalue decomposition)
 * - test to insure that results with double are binary compatible with result using Sdouble
 */

#endif //SHAMAN_DEMO_EIGEN_H