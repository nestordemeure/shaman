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

template<typename numberType, typename errorType, typename preciseType> class ErrorSum
{
public:
    // contains the sum of the error so far
    errorType totalError;

    // contains the error decomposed in composants (one per block encountered)
    // TODO might use a more efficient representation
    std::unordered_map<Tag,errorType> errors;

    //-------------------------------------------------------------------------

    /*
     * builds a new errorSum using the content of an existing errorSum
     */
    ErrorSum(std::unordered_map<Tag,errorType> errorsArg, errorType totalErrorArg): totalError(totalErrorArg), errors(errorsArg) {}

    /*
     * returns an errorSum with a single element (singleton)
     */
    ErrorSum(const Tag& name, errorType error): totalError(error)
    {
        errors[name] = error;
    }

    /*
     * returns an errorSum with a single element (singleton)
     */
    ErrorSum(errorType error): totalError(error)
    {
        Tag name = Block::currentBlock();
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
    operator std::string() const
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
     * TODO there might be a more efficient way to build the new unordered map
     */
    static inline ErrorSum unaryNeg(const ErrorSum& errorSum)
    {
        std::unordered_map<Tag,errorType> newErrors(errorSum.errors);
        transform(newErrors, [](errorType e){return -e;});
        errorType newtotalError = -errorSum.totalError;
        return ErrorSum(newErrors, newtotalError);
    }

    /*
     * *= scalar
     */
    inline void multByScalar(errorType scalar)
    {
        totalError *= scalar;
        transform(errors, [scalar](errorType e){return e * scalar;});
    }

    /*
     * /= scalar
     */
    inline void divByScalar(errorType scalar)
    {
        totalError /= scalar;
        transform(errors, [scalar](errorType e){return e / scalar;});
    }

    /*
     * += errors
     */
    inline void addErrors(const ErrorSum& errors2)
    {
        totalError += errors2.totalError;
        for(auto kv : errors2.errors)
        {
            errors[kv.first] += kv.second;
        }
    }

    /*
     * -= errors
     */
    inline void subErrors(const ErrorSum& errors2)
    {
        totalError -= errors2.totalError;
        for(auto kv : errors2.errors)
        {
            errors[kv.first] -= kv.second;
        }
    }

    /*
     * += scalar * errors
     */
    inline void addErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        totalError += scalar * errors2.totalError;
        for(auto kv : errors2.errors)
        {
            errors[kv.first] += kv.second * scalar;
        }
    }

    /*
     * -= scalar * errors
     */
    inline void subErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        totalError -= scalar * errors2.totalError;
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
    inline static void transform(std::unordered_map<Tag,errorType>& dict, FUN f)
    {
        for(auto kv : dict)
        {
            dict[kv.first] = f(kv.second);
        }
    }

    /*
     * given f, x and y such that f(x) = y, updates the errors
     * the error is updated by being proportionality distributed amongst elements
     *
     * the proportionality is not a perfect system (unless f is linear) but it preserves the sum of errors (which matters over all other properties)
     */
    /*
    template<typename FUN>
    void functionError(FUN f, numberType x, numberType y)
    {
        errorType previoustotalError = totalError();
        errorType xPrecise = x + previoustotalError;
        errorType yPrecise = f(xPrecise);
        errorType newTotalError = yPrecise - y;
        errorType scalingFactor = newTotalError / previoustotalError;
        multByScalar(scalingFactor);
    }
    */

};

#endif //SHAMAN_ERRORSUM_H
