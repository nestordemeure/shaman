
#include <chrono>
#include "shaman/Shaman.h"
// tests
#include "tests/test_eft.h"
// examples
#include "examples/tests.h"
#include "examples/legendre.h"
#include "examples/schrodinger.h"

//---------------------------------------------------------------------------------------
// MAIN

int main()
{
    auto begin = std::chrono::steady_clock::now();

    // tests
    test_eft();

    /*
    // various tests
    rumpTest();
    polynomialTest();
    fixedPointTest();
    kahanIdentity();
    mullerTest();

    // legendre
    legendre20Test();

    // Schrodinger equation
    Schrodinger numerov = Schrodinger();
    numerov.calculate();
    */

    // displays computation time
    auto end = std::chrono::steady_clock::now();
    auto elapsedSec = std::chrono::duration<double>(end - begin).count();
    std::cout << "Time elapsed = " << elapsedSec << 's' << std::endl;

    return 0;
}