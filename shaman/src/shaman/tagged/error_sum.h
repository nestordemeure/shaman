#pragma once

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <exception>
#include "tagger.h"

#ifndef SHAMAN_TAGNUMBER
#define SHAMAN_TAGNUMBER 25
#endif

/*
 * POD error_sum that has a maxComposantNumber in its definition
 * NOTE: adding a capacity to avoid doing computation on zero terms decrease performances
 */
template<typename errorType> class error_sum
{
private:
    // contains the error decomposed in composants (one per block encountered)
    // errors[tag] = error // if tag is out or range, error is 0
    std::array<errorType, SHAMAN_TAGNUMBER> errors;

public:

    /*
     * empty constructor : currently no error
     */
    explicit error_sum(): errors() {}

    /*
     * copy constructor that allows construction from another error_sum with the same number of composant number
     */
    template<typename errorType2>
    error_sum(const error_sum<errorType2>& errorSum2): errors(errorSum2.errors()) {}

    /*
     * returns an errorSum with a single element (singleton)
     */
    error_sum(Tag tag, errorType error): errors()
    {
        if(tag < SHAMAN_TAGNUMBER)
        {
            errors[tag] = error;
        }
        else
        {
            throw std::exception("SHAMAN: You have been using more than " + SHAMAN_TAGNUMBER + " tags. Please set SHAMAN_TAGNUMBER to a larger number.");
        }
    }

    /*
     * returns an errorSum with a single element (singleton)
     * uses the current tag
     */
    explicit error_sum(errorType error): errors()
    {
        Tag tag = CodeBlock::currentBlock();
        if(tag < SHAMAN_TAGNUMBER)
        {
            errors[tag] = error;
        }
        else
        {
            throw std::exception("SHAMAN: You have been using more than " + SHAMAN_TAGNUMBER + " tags. Please set SHAMAN_TAGNUMBER to a larger number.");
        }
    }

    /*
     * constructor that takes a function such that errors[i] = function(errorsum1[i])
     * NOTE: we do not construct errors with errors() so it starts non 0 initialized
     */
    template<typename FUN>
    error_sum(const error_sum& errorSum, FUN function)
    {
        std::transform(errorSum.errors.begin(), errorSum.errors.end(), errors.begin(), function);
    }

    /*
     * constructor that takes a function such that errors[i] = function(errorsum1[i], errorsum2[i])
     * NOTE: we do not construct errors with errors() so it starts non 0 initialized
     */
    template<typename FUN>
    error_sum(const error_sum& errorSum1, const error_sum& errorSum2, FUN function)
    {
        std::transform(errorSum1.errors.begin(), errorSum1.errors.end(), errorSum2.errors.begin(), errors.begin(), function);
    }

    //-------------------------------------------------------------------------
    // OPERATIONS

    /*
     * += error
     */
    void addError(errorType error)
    {
        Tag tag = CodeBlock::currentBlock();
        if(tag < SHAMAN_TAGNUMBER)
        {
            errors[tag] += error;
        }
        else
        {
            throw std::exception("SHAMAN: You have been using more than " + SHAMAN_TAGNUMBER + " tags. Please set SHAMAN_TAGNUMBER to a larger number.");
        }
    }

    /*
     * *= scalar
     */
    void multByScalar(errorType scalar)
    {
        std::transform(errors.begin(), errors.end(), errors.begin(), [scalar](errorType x){return x*scalar;});
    }

    /*
     * /= scalar
     */
    void divByScalar(errorType scalar)
    {
        std::transform(errors.begin(), errors.end(), errors.begin(), [scalar](errorType x){return x/scalar;});
    }

    /*
     * += errorComposants
     */
    void addErrors(const error_sum& errorSum2)
    {
        std::transform(errors.begin(), errors.end(), errorSum2.errors.begin(), errors.begin(), std::plus<errorType>());
    }

    /*
     * -= errorComposants
     */
    void subErrors(const error_sum& errorSum2)
    {
        std::transform(errors.begin(), errors.end(), errorSum2.errors.begin(), errors.begin(), std::minus<errorType>());
    }

    /*
     * += scalar * errorComposants
     */
    void addErrorsTimeScalar(const error_sum& errorSum2, errorType scalar)
    {
        std::transform(errors.begin(), errors.end(), errorSum2.errors.begin(), errors.begin(), [scalar](errorType e1, errorType e2){return e1 + e2*scalar;});
    }

    //-------------------------------------------------------------------------
    // DISPLAY

    /*
     * produces a readable string representation of the error terms
     * the error terms are expressed in percents of the sum of absolute errors, sorted from larger to smaller
     * the signs are preserved to better reflect compensations BUT the sum of percents is 100 only if you ignore signs
     * we display only the terms larger than a given percent
     */
    explicit operator std::string() const
    {
        int minErrorPercent = 5;

        // computes the sum of abs(error) and the sign of the sum of errors
        errorType totalAbsoluteError = 0.;
        errorType totalError = 0.;
        for(auto error : errors)
        {
            totalAbsoluteError += std::fabs(error);
            totalError += error;
        }
        // rectifies the sign of the sum of absolute errors to match the sign of the sum
        totalAbsoluteError = std::copysign(totalAbsoluteError, totalError);

        // collects the relevant data expressed in percent of the total error
        std::vector<std::pair<Tag, errorType>> data;
        bool droppedNonSignificantTerms = false;
        for(int tag = 0; tag < errors.size(); tag++)
        {
            errorType error = errors[tag];

            if (std::isnan(error) || std::isinf(error))
            {
                data.push_back(std::make_pair(tag, error));
            }
            else
            {
                int percent = (error*100.) / totalAbsoluteError;

                if(std::abs(percent) >= minErrorPercent)
                {
                    // TODO temporary full display that is not in %
                    data.push_back(std::make_pair(tag, error));
                    //data.push_back(std::make_pair(tag, percent));
                }
                else if (error != 0)
                {
                    droppedNonSignificantTerms = true;
                }
            }
        }

        // sorts the vector by abs(error) descending
        auto compare = [](const std::pair<Tag, errorType>& p1, const std::pair<Tag, errorType>& p2){return std::fabs(p1.second) > std::fabs(p2.second);};
        std::sort(data.begin(), data.end(), compare);

        // output stream
        std::ostringstream output;
        output << '[';

        // displays each other element prefixed by a ", " separator
        if(data.size() > 0)
        {
            auto kv = data[0];
            output << CodeBlock::nameOfTag(kv.first) << ':' << kv.second << '%';

            for(int i = 1; i < data.size(); i++)
            {
                kv = data[i];
                output << ", " << CodeBlock::nameOfTag(kv.first) << ':' << kv.second << '%';
            }
        }

        // adds '…' if we dropped some non significant terms
        if(droppedNonSignificantTerms)
        {
            output << "…";
        }

        output << ']';
        return output.str();
    }
};


