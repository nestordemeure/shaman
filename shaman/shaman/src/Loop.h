#ifndef SHAMAN_LOOP_H
#define SHAMAN_LOOP_H

#include "../Shaman.h"

//-------------------------------------------------------------------------------------------------

/*
 * I need a foreach macro
 * maybe using https://github.com/swansontec/map-macro
 * or no macro at all
 */

// creates temporary storage for some variables
// TODO how will it react when given no arguments ?
#define VAR_TEMPORARY_STORAGE(X,...) \
    auto temporary_##X = X; \
    VAR_TEMPORARY_STORAGE(__VA_ARGS__) \

// macro that saves some variable in their temporary storage
// TODO how will it react when given no arguments ?
#define VAR_SAVE(X,...) \
    temporary_##X = X; \
    VAR_SAVE(__VA_ARGS__) \

// take variables and correct them with the dedicated function
// TODO how will it react when given no arguments ?
#define VAR_CORRECTION(X, ...) \
    X = loopErrorCorrection(X, temporary_##X) \
    VAR_CORRECTION(__VA_ARGS__) \

//-----

// macro that initialise temporary variables for the loop
#define LOOP_BEGIN(...) \
    bool early_exit = false; \
    bool late_exit = false; \
    PairBool cond; \
    VAR_TEMPORARY_STORAGE(__VA_ARGS__) \

// macro that
#define LOOP_COND(COND, ...) \
    ( \
        cond = COND; \
        if((!early_exit) && isEarlyExit(cond)) \
        { \
            VAR_SAVE(__VA_ARGS__); \
            early_exit = true; \
        }; \
        late_exit |= isLateExit(cond); \
        cond.ieee \
    ) \

// macro that corrects the variables with the temporary variables
#define LOOP_END(...) \
    if (late_exit) \
    { \
        std::cout << "WARNING Late Exit" << std::endl; \
    } \
    else if (early_exit) \
    { \
        std::cout << "WARNING Early Exit" << std::endl; \
        VAR_CORRECTION(X, ...) \
    } \

//-------------------------------------------------------------------------------------------------

struct PairBool { bool ieee; bool corrected; };

static bool isEarlyExit(PairBool cond)
{
    return (!cond.corrected) && cond.ieee;
}

static bool isLateExit(PairBool cond)
{
    return cond.corrected && (!cond.ieee);
}

// <
templated static PairBool lower(const Snum& n1, const Snum& n2)
{
    bool ieee = n1.number < n2.number;
    bool corrected = n1.corrected_number() < n2.corrected_number();

    return {ieee, corrected};
};

//-------------------------------------------------------------------------------------------------

templated static Snum loopErrorCorrection(const Snum& n, const Snum temporaryN)
{
    numberType number = n.number;
    errorType error = temporaryN.corrected_number() - n.number;
    return Snum(number,error);
}

#endif //SHAMAN_LOOP_H