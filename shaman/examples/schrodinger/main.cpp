#include <chrono>

#include <shaman.h>

#include "schrodinger.h"

//---------------------------------------------------------------------------------------
// MAIN

int main()
{
    auto begin = std::chrono::steady_clock::now();


    // Schrodinger equation
    Schrodinger numerov = Schrodinger();
    numerov.calculate();

    // displays computation time
    auto end = std::chrono::steady_clock::now();
    auto elapsedSec = std::chrono::duration<double>(end - begin).count();
    std::cout << "Time elapsed = " << elapsedSec << 's' << std::endl;

    Shaman::displayUnstableBranches();

    return 0;
}