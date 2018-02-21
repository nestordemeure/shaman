
//#include <blas1.hpp>
#include <cmath>
#include "exact.h"

/*
 * computes the sum of the elements in the vector
 */
double exact::sum(std::vector<double> V)
{
    /*
    // arguments for the exBLAS function
    const int increment = 1;
    const int offset = 0;
    const int floatingPointExpansionSize = 1; // uses only superaccumulator if set to 1
    const bool earlyStop = false;

    return exsum(V.size(), V.data(), increment, offset, floatingPointExpansionSize, earlyStop);
    */
    throw("No exblass here.");
}

/*
 * compute the dot product of two vector
 */
double exact::dotProduct(std::vector<double> V, std::vector<double> W)
{
    /*
    // TODO add an openCL detection flag
    // arguments for the exBLAS function (GPU only)
    const int incrementV = 1;
    const int offsetV = 0;
    const int incrementW = 1;
    const int offsetW = 0;
    const int floatingPointExpansionSize = 1; // uses only superaccumulator if set to 1
    const bool earlyStop = false;

    return exdot(V.size(), V.data(), incrementV, offsetV, W.data(), incrementW, offsetW, floatingPointExpansionSize, earlyStop);
    */

    // solution for cpu only installation of exblas
    // uses an EFT to turn the dot product into a summation problem
    // could be more efficient
    int n = V.size();
    std::vector<double> eft(2*n);

    for (int i = 0; i < n; i++)
    {
        double v = V[i];
        double w = W[i];
        double product = v*w;
        double error = std::fma(v,w,-product);

        eft[2*i] = product;
        eft[2*i + 1] = error;
    }

    return exact::sum(eft);
}
