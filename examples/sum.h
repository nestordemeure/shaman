//
// Created by demeuren on 26/01/18.
//

#ifndef COMPENSATIONS_SUM_H
#define COMPENSATIONS_SUM_H

#include <algorithm>
#include <functional>
#include <random>
#include "../shaman/Shaman.h"
#include "various.h"

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

    LOCAL_BLOCK("loop");
    // usual sum
    Sdouble sum = 0.0;
    // kahan sum
    Sdouble kahanSum = 0.0;
    Sdouble compensation = 0.0;
    // improved kahan sum
    Sdouble improvedSum = 0.0;
    Sdouble compensation2 = 0.0;
    for(Sdouble x : data)
    {
        // usual sum
        sum += x;

        // kahan sum
        Sdouble y = x - compensation;
        Sdouble t = kahanSum + y;
        compensation = (t - kahanSum) - y;
        kahanSum = t;

        // improved kahan
        Sdouble t2 = improvedSum + x;
        if(Sstd::abs(improvedSum) >= Sstd::abs(x))
        {
            compensation2 += (improvedSum - t2) + x;
        }
        else
        {
            compensation2 += (x - t2) + improvedSum;
        }
        improvedSum = t2;
    }

    LOCAL_BLOCK("final_compensation");
    improvedSum += compensation2;

    std::cout << "sum: " << sum << std::endl;
    std::cout << "improved_sum: " << improvedSum << std::endl;
    std::cout << "kahan_sum: " << kahanSum << std::endl;
}

void parralelSum(int size)
{
    // data generation
    std::random_device rnd_device;
    std::mt19937 mersenne_engine(rnd_device()); // Specify the engine and distribution.
    std::uniform_real_distribution<double> dist(1.0e10-15, 2.0e-15);
    auto gen = std::bind(dist, mersenne_engine);
    std::vector<double> data(size);
    generate(begin(data), end(data), gen);

    // sequential sum
    Sdouble seqSum = 0;
    for(Sdouble x : data)
    {
        seqSum += x;
    }
    std::cout << "sequential sum\t:\t" << seqSum << std::endl;

    #ifdef _OPENMP
    // parrallel sum with reduce
    /*
    number parSumR = 0;
    #pragma omp parallel for reduction(+:parSumR)
    for(unsigned int i = 0; i < data.size(); i++)
    {
        parSumR += data[i];
    }
    std::cout << "parallel sum\t:\t" << parSumR << " (with reduce)" << std::endl;
    */

    // parrallel sum without reduce
    Sdouble parSum = 0;
    #pragma omp parallel
    {
        Sdouble localParSum = 0;

        #pragma omp for
        for(unsigned int i = 0; i < data.size(); i++)
        {
            localParSum += data[i];
        }

        #pragma omp critical
        parSum += localParSum;
    };
    std::cout << "parallel sum\t:\t" << parSum << " (without reduce)" << std::endl;
    #endif //_OPENMP
}

#endif //COMPENSATIONS_SUM_H
