//
// Created by demeuren on 04/06/18.
//

#ifndef SHAMAN_ERRORSUM_H
#define SHAMAN_ERRORSUM_H

#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>
#include "Tagger.h"

template<typename errorType> class ErrorSum
{
public:
    // contains the error decomposed in composants (one per block encountered)
    // TODO might use a more efficient representation : a sparse vector
    std::unordered_map<Shaman::Tag,errorType> errors;

    //-------------------------------------------------------------------------

    /*
     * empty constructor : currently no error
     */
    ErrorSum() = default;

    /*
     * returns an errorSum with a single element (singleton)
     */
    ErrorSum(const Shaman::Tag& name, errorType error)
    {
        errors[name] = error;
    }

    /*
     * returns an errorSum with a single element (singleton)
     * uses the current tag
     */
    explicit ErrorSum(errorType error)
    {
        Shaman::Tag name = Block::currentBlock();
        errors[name] = error;
    }

    /*
     * produces a readable string representation of the error terms
     *
     * TODO
     * sort by abs
     * convert to key:value (value being printed with 2 significant digits)
     * concats with separating ', '
     *
     * maybe keep at most the ten most important terms (and displays the number of terms ?)
     */
    explicit operator std::string() const
    {
        std::ostringstream output;

        output << std::scientific << std::setprecision(2) << '[' << ' ';
        for(auto kv : errors)
        {
            output << kv.first << ':' << kv.second << ' ';
        }
        output << ']';

        return output.str();
    }

    //-------------------------------------------------------------------------

    /*
     * ~-
     */
    static inline ErrorSum unaryNeg(const ErrorSum& errorSum)
    {
        ErrorSum result(errorSum);
        transform(result.errors, [](errorType e){return -e;});
        return result;
    }

    /*
     * *= scalar
     */
    inline void multByScalar(errorType scalar)
    {
        transform(errors, [scalar](errorType e){return e * scalar;});
    }

    /*
     * /= scalar
     */
    inline void divByScalar(errorType scalar)
    {
        transform(errors, [scalar](errorType e){return e / scalar;});
    }

    /*
     * += errorComposants
     */
    inline void addErrors(const ErrorSum& errors2)
    {
        for(auto kv : errors2.errors)
        {
            errors[kv.first] += kv.second;
        }
    }

    /*
     * += error
     */
    inline void addError(errorType error)
    {
        Shaman::Tag name = Block::currentBlock();
        errors[name] += error;
    }

    /*
     * -= errorComposants
     */
    inline void subErrors(const ErrorSum& errors2)
    {
        for(auto kv : errors2.errors)
        {
            errors[kv.first] -= kv.second;
        }
    }

    /*
     * += scalar * errorComposants
     */
    inline void addErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        for(auto kv : errors2.errors)
        {
            errors[kv.first] += kv.second * scalar;
        }
    }

    /*
     * -= scalar * errorComposants
     */
    inline void subErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        for(auto kv : errors2.errors)
        {
            errors[kv.first] -= kv.second * scalar;
        }
    }

    //-------------------------------------------------------------------------

    /*
     * homemade implementation of std::transform since I was unable to use the original
     * TODO find a way to use std::transform
     */
    template<typename FUN>
    inline static void transform(std::unordered_map<Shaman::Tag,errorType>& dict, FUN f)
    {
        for(auto kv : dict)
        {
            dict[kv.first] = f(kv.second);
        }
    }
};

#endif //SHAMAN_ERRORSUM_H
