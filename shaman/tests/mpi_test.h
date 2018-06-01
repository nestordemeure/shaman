//
// Created by demeuren on 22/02/18.
//

#ifndef SHAMAN_DEMO_MPI_TEST_H
#define SHAMAN_DEMO_MPI_TEST_H

#include <mpi.h>
#include <random>
#include <functional>
#include <algorithm>
#include "../shaman/lib/Shaman_mpi.h"

/*
 * inspired by :
 * https://github.com/wesleykendall/mpitutorial/blob/gh-pages/tutorials/mpi-reduce-and-allreduce/code/reduce_avg.c
 *
 * TODO we should send a shaman type, get it back and check wether it was modified by MPI
 * compile with mpic++, run with mpirun
 */
void mpiSum(int size)
{
    MPI_Shaman_Init(NULL, NULL);

    // get process number
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // data generation
    std::mt19937 mersenne_engine(world_rank);
    std::uniform_real_distribution<double> dist((1+world_rank)*1.0e10 - 15, (1+world_rank)*2.0e-15);
    auto gen = std::bind(dist, mersenne_engine);
    std::vector<double> data(size);
    std::generate(begin(data), end(data), gen);

    // ----- MPI -----

    // local sequential sum
    double local_sum = 0;
    for(double x : data)
    {
        local_sum += x;
    }
    std::cout << "local double sum\t:\t" << local_sum << "\tprocess:" << (world_rank+1)  << '/' << world_size << std::endl;

    // Reduce all of the local sums into the global sum
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (world_rank == 0)
    {
        std::cout << "global double sum\t:\t" << global_sum << '\n' << std::endl;
    }

    // ----- MPI + SHAMAN -----

    // local sequential sum
    Sdouble local_par_sum = 0;
    for(Sdouble x : data)
    {
        local_par_sum += x;
    }
    std::cout << "local shaman sum\t:\t" << local_par_sum << "\tprocess:" << (world_rank+1)  << '/' << world_size << std::endl;

    // Reduce all of the local sums into the global sum
    Sdouble global_par_sum;
    MPI_Reduce(&local_par_sum, &global_par_sum, 1, MPI_SDOUBLE, MPI_SSUM, 0, MPI_COMM_WORLD);
    if (world_rank == 0)
    {
        std::cout << "global shaman sum\t:\t" << global_par_sum << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Shaman_Finalize();
}

#endif //SHAMAN_DEMO_MPI_TEST_H