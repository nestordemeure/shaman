
#include <chrono>
#include "shaman/Shaman.h"
#include "examples/tests.h"
#include "examples/schrodinger.h"
#include "examples/legendre.h"

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