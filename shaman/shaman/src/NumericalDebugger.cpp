//
// Created by nestor on 02/02/18.
//

#include <cstdlib>
#include <iostream>
#include "NumericalDebugger.h"

/*
 * adds a function that will display the number of unstability and cancelation when the code terminate
 */
#ifdef NUMERICAL_DEBUGGER_ENABLED
int init = []()
{
    std::atexit(NumericalDebugger::printUnstabilities);
    return 0;
}();
#endif //NUMERICAL_DEBUGGER_ENABLED

/*
 * keep a tab of the number of instabilities and cancelations in the code
 */
int NumericalDebugger::unstabilityCount = 0;
int NumericalDebugger::cancelations = 0;
int NumericalDebugger::restorations = 0;
int NumericalDebugger::numericalZeros = 0;
int NumericalDebugger::unstableDivisions = 0;
int NumericalDebugger::unstableMultiplications = 0;
int NumericalDebugger::unstableFunctions = 0;
int NumericalDebugger::unstablePowerFunctions = 0;
int NumericalDebugger::unstableBranchings = 0;
bool NumericalDebugger::shouldDisplay = true; // useful for MPI and debugging

/*
 * print the number of instabilities detected
 */
void NumericalDebugger::printUnstabilities()
{
    if (shouldDisplay)
    {
        std::cout << '\n' << "*** SHAMAN ***" << '\n';

        #ifdef _OPENMP // sum unstability counters
        std::cout << "(With openMP)" << std::endl;
        int unstabilityCount_red = 0;
        int numericalZeros_red = 0;
        int cancelations_red = 0;
        int restorations_red = 0;
        int unstableDivisions_red = 0;
        int unstableMultiplications_red = 0;
        int unstableFunctions_red = 0;
        int unstablePowerFunctions_red = 0;
        int unstableBranchings_red = 0;
        #pragma omp parallel reduction(+:unstabilityCount_red, numericalZeros_red, cancelations_red, unstableDivisions_red, unstableMultiplications_red, \
                                         unstableFunctions_red, unstablePowerFunctions_red, unstableBranchings_red, restorations_red)
        {
            unstabilityCount_red += unstabilityCount;
            numericalZeros_red += numericalZeros;
            cancelations_red += cancelations;
            restorations_red += restorations;
            unstableDivisions_red += unstableDivisions;
            unstableMultiplications_red += unstableMultiplications;
            unstableFunctions_red += unstableFunctions;
            unstablePowerFunctions_red += unstablePowerFunctions;
            unstableBranchings_red += unstableBranchings;
        }
        unstabilityCount = unstabilityCount_red;
        cancelations = cancelations_red;
        restorations = restorations_red;
        numericalZeros = numericalZeros_red;
        unstableDivisions = unstableDivisions_red;
        unstableMultiplications = unstableMultiplications_red;
        unstableFunctions = unstableFunctions_red;
        unstablePowerFunctions = unstablePowerFunctions_red;
        unstableBranchings = unstableBranchings_red;
        #endif //_OPENMP

        if (unstabilityCount == 0)
        {
            std::cout << "No instability detected" << std::endl;
        }
        else
        {
            // detection of a deep numerical anomaly that might throw the error model off
            if ((unstableDivisions > 0) || (unstablePowerFunctions > 0) || (unstableMultiplications > 0) || (unstableFunctions > 0))
            {
                std::cout << "WARNING : the self-validation detects major problem(s)." << '\n'
                          << "The results are NOT guaranteed" << '\n';
            }

            std::cout << "There are " << unstabilityCount << " numerical unstabilities" << '\n'
                      #ifdef NUMERICAL_ZERO_DEBUGGER
                      << numericalZeros << " NUMERICAL ZERO(S)" << '\n'
                      #endif
                      #ifdef CANCELATION_DEBUGGER
                      << cancelations << " CANCELLATION(S)" << '\n'
                      #endif
                      #ifdef UNSTABLE_OP_DEBUGGER
                      << unstableDivisions << " UNSTABLE DIVISION(S)" << '\n'
                      << unstableMultiplications << " UNSTABLE MULTIPLICATION(S)" << '\n'
                      << unstableFunctions << " UNSTABLE MATHEMATICAL FUNCTION(S)" << '\n'
                      << unstablePowerFunctions << " UNSTABLE POWER FUNCTION(S)" << '\n'
                      #endif
                      #ifdef UNSTABLE_BRANCH_DEBUGGER
                      << unstableBranchings << " UNSTABLE BRANCHING(S)" << '\n'
                      #endif
                      << std::endl;
        }
    }
}

/*
 * function called when an instability is detected
 *
 * put a breakpoint here to stop the debugger at each unstabilities
 */
void NumericalDebugger::unstability()
{
    unstabilityCount++;
};