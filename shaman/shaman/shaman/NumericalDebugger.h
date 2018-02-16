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
    static unsigned int unstabilityCount;
    static unsigned int unstablePowerFunctions;
    static unsigned int unstableDivisions;
    static unsigned int unstableMultiplications;
    static unsigned int unstableFunctions;
    static unsigned int unstableBranchings;
    static unsigned int cancelations;

    // functions
    static void unstability();
    static void printUnstabilities();
};

#endif //COMPENSATIONS_NUMERICALDEBUGGER_H
#endif //NUMERICAL_DEBUGGER