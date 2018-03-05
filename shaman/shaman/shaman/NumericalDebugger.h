//
// Created by nestor on 02/02/18.
//

#ifndef COMPENSATIONS_NUMERICALDEBUGGER_H
#define COMPENSATIONS_NUMERICALDEBUGGER_H

//-----------------------------------------------------------------------------
// MACROS

// general macro that enable most counters
#ifdef NUMERICAL_DEBUGGER
    #define NUMERICAL_ZERO_DEBUGGER // TODO
    #define CANCELATION_DEBUGGER
    #define UNSTABLE_OP_DEBUGGER
    #define UNSTABLE_BRANCH_DEBUGGER
    //#define RESTAURATION_DEBUGGER // TODO
#endif //NUMERICAL_DEBUGGER

// is at least one counter enabled ?
#if defined(NUMERICAL_ZERO_DEBUGGER) || defined(CANCELATION_DEBUGGER) || defined(UNSTABLE_OP_DEBUGGER) \
                                     || defined(UNSTABLE_BRANCH_DEBUGGER) || defined(RESTAURATION_DEBUGGER)
    #define NUMERICAL_DEBUGGER_ENABLED
#endif

// is usefull to keep track of the fact that a number has no significative digits ?
#if defined(NUMERICAL_ZERO_DEBUGGER) || defined(UNSTABLE_OP_DEBUGGER)
    #define NUMERICAL_ZERO_FIELD_ENABLED
#endif

/*
 * TODO :
 * - add a boolean that stores wheter a Snum is a numerical zero
 * - add a test to see if numerical zeros where created during execution
 * - implement and add the detection and handling of restauration
 *   (error should not grow on a restaurarion if enable)
 */

//-----------------------------------------------------------------------------
// CLASS

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
    // numerical zeros
    static int numericalZeros;
    // cancelations
    static int cancelations;
    // unstable operations
    static int unstablePowerFunctions;
    static int unstableDivisions;
    static int unstableMultiplications;
    static int unstableFunctions;
    // unstable branching
    static int unstableBranchings;
    #ifdef _OPENMP
    #pragma omp threadprivate (NumericalDebugger::unstabilityCount, NumericalDebugger::unstablePowerFunctions, NumericalDebugger::unstableDivisions, \
                           NumericalDebugger::unstableMultiplications, NumericalDebugger::unstableFunctions, NumericalDebugger::unstableBranchings, \
                           NumericalDebugger::cancelations, NumericalDebugger::numericalZeros)
    #endif //_OPENMP

    // functions
    static void unstability();
    static void printUnstabilities();
};

#endif //COMPENSATIONS_NUMERICALDEBUGGER_H