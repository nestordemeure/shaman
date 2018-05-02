//
// Created by nestor on 02/02/18.
//

#ifndef COMPENSATIONS_NUMERICALDEBUGGER_H
#define COMPENSATIONS_NUMERICALDEBUGGER_H

#include <cstdlib>
#include <iostream>

//-----------------------------------------------------------------------------
// MACROS

/*
 * dependancy between flags :
 *
 * 0 NUMERICAL_DEBUGGER
 *    1 UNSTABLE_BRANCH_DEBUGGER
 *       5 NUMERICAL_DEBUGGER_ENABLED
 *    2 CANCELATION_DEBUGGER
 *       5 NUMERICAL_DEBUGGER_ENABLED
 *    3 NUMERICAL_ZERO_DEBUGGER
 *       5 NUMERICAL_DEBUGGER_ENABLED
 *       7 NUMERICAL_ZERO_FIELD_ENABLED
 *    4 UNSTABLE_OP_DEBUGGER
 *       5 NUMERICAL_DEBUGGER_ENABLED
 *       6 DOUBT_LEVEL_FIELD_ENABLED
 *          7 NUMERICAL_ZERO_FIELD_ENABLED
 */

// general macro that enable all specific debuggers
#ifdef NUMERICAL_DEBUGGER
    #define NUMERICAL_ZERO_DEBUGGER
    #define CANCELATION_DEBUGGER
    #define UNSTABLE_OP_DEBUGGER
    #define UNSTABLE_BRANCH_DEBUGGER
#endif //NUMERICAL_DEBUGGER

// should we activate the debugger ?
#if defined(NUMERICAL_ZERO_DEBUGGER) || defined(CANCELATION_DEBUGGER) || defined(UNSTABLE_OP_DEBUGGER) \
                                     || defined(UNSTABLE_BRANCH_DEBUGGER)
    #define NUMERICAL_DEBUGGER_ENABLED
#endif

// is it useful to keep track of the number of unstable operations behind a result ?
#if defined(UNSTABLE_OP_DEBUGGER)
    #define DOUBT_LEVEL_FIELD_ENABLED
#endif

// is it useful to keep track of the fact that a number has no significative digits ?
#if defined(NUMERICAL_ZERO_DEBUGGER) || defined(UNSTABLE_OP_DEBUGGER) || defined(DOUBT_LEVEL_FIELD_ENABLED)
    #define NUMERICAL_ZERO_FIELD_ENABLED
#endif

//-----------------------------------------------------------------------------
// CLASS

/*
 * Tracks the number of cancelations and unstabilities in the code
 * Allows the debugging of numerical error by adding a breakpoint on the unstability function
 */
namespace NumericalDebugger
{
    /*
     * keep a tab of the number of instabilities and cancelations in the code
     */
    static int unstabilityCount = 0;
    static int numericalZeros = 0;
    static int cancelations = 0;
    static int restorations = 0;
    static int unstablePowerFunctions = 0;
    static int unstableDivisions = 0;
    static int unstableMultiplications = 0;
    static int unstableFunctions = 0;
    static int unstableBranchings = 0;
    static bool shouldDisplay = true; // useful for MPI and debugging
    #ifdef _OPENMP
    #pragma omp threadprivate (NumericalDebugger::unstabilityCount, NumericalDebugger::unstablePowerFunctions, NumericalDebugger::unstableDivisions, \
                           NumericalDebugger::unstableMultiplications, NumericalDebugger::unstableFunctions, NumericalDebugger::unstableBranchings, \
                           NumericalDebugger::cancelations, NumericalDebugger::restorations, NumericalDebugger::numericalZeros)
    #endif //_OPENMP

    /*
     * function called when an instability is detected
     */
    static void unstability()
    {
        unstabilityCount++; // Put a breakpoint here to stop the debugger at each unstabilities
    };

    /*
     * print the number of instabilities detected
     */
    static void printUnstabilities()
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
};

/*
 * adds a function that will display the number of unstability and cancelation when the code terminate
 */
#ifdef NUMERICAL_DEBUGGER_ENABLED
static int init = []()
{
    std::atexit(NumericalDebugger::printUnstabilities);
    return 0;
}();
#endif //NUMERICAL_DEBUGGER_ENABLED

#endif //COMPENSATIONS_NUMERICALDEBUGGER_H