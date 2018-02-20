//
// Created by demeuren on 26/01/18.
//

#ifndef COMPENSATIONS_SUM_H
#define COMPENSATIONS_SUM_H

#include <algorithm>
#include <functional>
#include <random>
#include "../shaman/shaman/Shaman.h"
#include "exact/exact.h"
#include "tests.h"

/*
 * computing time for 50000000 elements on nestor's laptop using clang++5.0
 * S : 10s
 * double : 6.5s
 * => 1.5 slower
 *
 * on TERATEC's desktop + inline
 * S : 2.4s
 * double : 1.7s
 * => 1.4x slower
 * S + numerical debugging : 29s
 * => 17x slower
 */

void Compensatedsum(int size)
{
    // data generation
    std::random_device rnd_device;
    std::mt19937 mersenne_engine(rnd_device()); // Specify the engine and distribution.
    std::uniform_real_distribution<double> dist(1.0e10-15, 2.0e-15);
    auto gen = std::bind(dist, mersenne_engine);
    std::vector<double> data(size);
    generate(begin(data), end(data), gen);

    // usual sum
    number sum = 0.0;
    // kahan sum
    number kahanSum = 0.0;
    number compensation = 0.0;
    // improved kahan sum
    number improvedSum = 0.0;
    number compensation2 = 0.0;
    for(double x : data)
    {
        // usual sum
        sum += x;

        // kahan sum
        number y = x - compensation;
        number t = kahanSum + y;
        compensation = (t - kahanSum) - y;
        kahanSum = t;

        // improved kahan
        number t2 = improvedSum + x;
        if( fabs(improvedSum) >= fabs(x) )
        {
            compensation2 += (improvedSum - t2) + x;
        }
        else
        {
            compensation2 += (x - t2) + improvedSum;
        }
        improvedSum = t2;
    }
    improvedSum += compensation2;

    // compute the exact sum using exblas
    double exactSum = exact::sum(data);

    std::cout << "sum: " << sum << std::endl;
    displayError(sum, exactSum);

    std::cout << "improved_sum: " << improvedSum << std::endl;
    displayError(improvedSum, exactSum);

    std::cout << "kahan_sum: " << kahanSum << std::endl;
    displayError(kahanSum, exactSum);
}

#endif //COMPENSATIONS_SUM_H
