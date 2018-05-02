
#include <chrono>
#include "shaman/Shaman.h"
#include "tests/tests.h"
#include "tests/sum.h"
#include "tests/schrodinger.h"
#include "tests/cadna_test.h"
#include "tests/legendre.h"
#include "tests/eigen.h"
//#include "tests/mpi_test.h"

//---------------------------------------------------------------------------------------
// MAIN

int main()
{
    auto begin = std::chrono::steady_clock::now();

    // various tests
    //rumpTest();
    //polynomialTest();
    //fixedPointTest();
    //kahanIdentity();
    //mullerTest();

    // sums
    //Compensatedsum(50000000);
    //parralelSum(50000000);
    //mpiSum(50000000);

    // Schrodinger equation
    Schrodinger numerov = Schrodinger();
    numerov.calculate();

    // legendre
    //legendre20Test();

    // CADNA
    //cadna_init(-1);
    //polynomial();
    //secondOrder();
    //hilbert();
    //muller();
    //newton();
    //gauss();
    //jacobi();
    //falsePositive();
    //alternatingCounter();
    //trigoTest();
    //cadna_end();

    // eigen
    //eigenDemo();

    // displays computation time
    auto end = std::chrono::steady_clock::now();
    auto elapsedSec = std::chrono::duration<double>(end - begin).count();
    std::cout << "Time elapsed = " << elapsedSec << 's' << std::endl;

    return 0;
}