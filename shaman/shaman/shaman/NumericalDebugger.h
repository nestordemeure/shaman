//
// Created by nestor on 02/02/18.
//

#ifndef COMPENSATIONS_NUMERICALDEBUGGER_H
#define COMPENSATIONS_NUMERICALDEBUGGER_H

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
    // restorations
    static int restorations;
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
                           NumericalDebugger::cancelations, NumericalDebugger::restorations, NumericalDebugger::numericalZeros)
    #endif //_OPENMP

    // functions
    static void unstability();
    static void printUnstabilities();
};

#endif //COMPENSATIONS_NUMERICALDEBUGGER_H