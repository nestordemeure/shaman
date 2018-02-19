
#include <chrono>
#include "shaman/shaman/Shaman.h"
#include "tests/tests.h"
#include "tests/sum.h"
#include "tests/schrodinger.h"
#include "tests/cadna.h"
#include "tests/legendre.h"

//---------------------------------------------------------------------------------------
// MAIN

int main()
{
    cadna_init(-1);
    auto begin = std::chrono::steady_clock::now();

    // various tests
    /*
    rumpTest();
    polynomialTest();
    fixedPointTest();
    //kahanIdentity();
    mullerTest();
    */

    // sums
    //Compensatedsum(50000000);

    // Schrodinger equation
    //Schrodinger numerov = Schrodinger();
    //numerov.calculate();

    // legendre
    //legendre20Test();

    // CADNA double
    polynomial();
    secondOrder();
    hilbert();
    muller();
    newton();
    falsePositive();
    //gauss();
    //jacobi();

    // displays computation time
    auto end = std::chrono::steady_clock::now();
    auto elapsedSec = std::chrono::duration<double>(end - begin).count();
    std::cout << "Time elapsed = " << elapsedSec << 's' << std::endl;

    cadna_end();
    return 0;
}