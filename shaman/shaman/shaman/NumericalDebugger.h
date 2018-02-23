//
// Created by nestor on 02/02/18.
//

#ifdef NUMERICAL_DEBUGGER
#ifndef COMPENSATIONS_NUMERICALDEBUGGER_H
#define COMPENSATIONS_NUMERICALDEBUGGER_H

/*
 * Tracks the number of cancelations and unstabilities in the code
 *
 * Allows the debugging of numerical error by adding a breakpoint on the unstability function
 */
class NumericalDebugger
{
public:
    // unstability counters
    static bool shouldDisplay;
    static int unstabilityCount;
    static int unstablePowerFunctions;
    static int unstableDivisions;
    static int unstableMultiplications;
    static int unstableFunctions;
    static int unstableBranchings;
    static int cancelations;
    #ifdef _OPENMP
    #pragma omp threadprivate (NumericalDebugger::unstabilityCount, NumericalDebugger::unstablePowerFunctions, NumericalDebugger::unstableDivisions, \
                           NumericalDebugger::unstableMultiplications, NumericalDebugger::unstableFunctions, NumericalDebugger::unstableBranchings, \
                           NumericalDebugger::cancelations)
    #endif //_OPENMP

    // functions
    static void unstability();
    static void printUnstabilities();
};

#endif //COMPENSATIONS_NUMERICALDEBUGGER_H
#endif //NUMERICAL_DEBUGGER