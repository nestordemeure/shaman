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
    explicit ErrorSum() = default;

    /*
     * returns an errorSum with a single element (singleton)
     */
    explicit ErrorSum(const Shaman::Tag& name, errorType error)
    {
        if(error != 0)
        {
            errors[name] = error;
        }
    }

    /*
     * returns an errorSum with a single element (singleton)
     * uses the current tag
     */
    explicit ErrorSum(errorType error)
    {
        if(error != 0)
        {
            Shaman::Tag name = Block::currentBlock();
            errors[name] = error;
        }
    }

    /*
     * produces a readable string representation of the error terms
     * TODO maybe keep at most the ten most important terms (and displays the number of terms ?) or just display terms that have an impact on the number of digits
     */
    explicit operator std::string() const
    {
        std::ostringstream output;
        unsigned int defaultPrecision = 2;
        output << std::scientific << std::setprecision(defaultPrecision) << '[';

        if(errors.empty())
        {
            output << "no-error";
        }
        else
        {
            // load the data in a vector
            std::vector<std::pair<Shaman::Tag, errorType>> data;
            for(auto kv : errors)
            {
                data.push_back(kv);
            }

            // sorts the vector by abs(error) descending
            auto compare = [](const std::pair<Shaman::Tag, errorType>& p1, const std::pair<Shaman::Tag, errorType>& p2){return std::abs(p1.second) > std::abs(p2.second);};
            std::sort(data.begin(), data.end(), compare);

            // add the first element
            auto kv = data[0];
            output << Block::nameOfTag(kv.first) << ':' << kv.second;

            // add each other element prefixed by a ", " separator
            for(int i = 1; i < data.size(); i++)
            {
                kv = data[i];
                output << ", " << Block::nameOfTag(kv.first) << ':' << kv.second;
            }
        }

        output << ']';
        return output.str();
    }

    //-------------------------------------------------------------------------

    /*
     * ~-
     */
    inline void unaryNeg()
    {
        transform(errors, [](errorType e){return -e;});
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
     * += error
     */
    inline void addError(errorType error)
    {
        Shaman::Tag name = Block::currentBlock();
        errors[name] += error;
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
