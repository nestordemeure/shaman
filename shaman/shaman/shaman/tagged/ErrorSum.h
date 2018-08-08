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
#include <iostream>
#include "Tagger.h"
#include "memoryStore.h"

template<typename errorType> class ErrorSum
{
public:
    // contains the error decomposed in composants (one per block encountered)
    // errors[tag] = error // if tag is out or range, error is 0
    std::vector<errorType>* errors;

    //-------------------------------------------------------------------------
    // CONSTRUCTORS

    /*
     * empty constructor : currently no error
     */
    explicit ErrorSum(): errors(MemoryStore<errorType>::getVector()) {}

    /*
     * copy constructor
     * WARNING this constructor needs to do a deep copy (which is not the default)
     */
    ErrorSum(const ErrorSum& errorSum2): errors(MemoryStore<errorType>::getVector())
    {
        errors->assign(errorSum2.errors->begin(), errorSum2.errors->end()); // assign insures that we do not drop the capacity (undefined behaviour for = operator)
    }

    /*
     * copy assignment
     * WARNING this constructor needs to do a deep copy (which is not the default)
     */
    ErrorSum& operator=(const ErrorSum& errorSum2)
    {
        *errors = *(errorSum2.errors); // cannot use 'assign' because errors might have existing values out of errorSum2.size()
        return *this;
    };

    /*
     * returns an errorSum with a single element (singleton)
     */
    explicit ErrorSum(Tag tag, errorType error): errors(MemoryStore<errorType>::getVector())
    {
        errors->resize(tag+1);
        (*errors)[tag] = error;
    }

    /*
     * returns an errorSum with a single element (singleton)
     * uses the current tag
     */
    explicit ErrorSum(errorType error): errors(MemoryStore<errorType>::getVector())
    {
        Tag tag = Block::currentBlock();
        errors->resize(tag+1);
        (*errors)[tag] = error;
    }

    /*
     * destructor
     */
    ~ErrorSum()
    {
        MemoryStore<errorType>::releaseVector(errors);
        errors = nullptr;
    }

    //-------------------------------------------------------------------------
    // OPERATIONS

    /*
     * ~-
     */
    void unaryNeg()
    {
        std::transform(errors->begin(), errors->end(), errors->begin(), [](errorType x){return -x;});
    }

    /*
     * *= scalar
     */
    void multByScalar(errorType scalar)
    {
        std::transform(errors->begin(), errors->end(), errors->begin(), [scalar](errorType x){return x*scalar;});
    }

    /*
     * /= scalar
     */
    void divByScalar(errorType scalar)
    {
        std::transform(errors->begin(), errors->end(), errors->begin(), [scalar](errorType x){return x/scalar;});
    }

    /*
     * += error
     */
    void addError(errorType error)
    {
        Tag tag = Block::currentBlock();

        // insures that errors is big enough to store the results
        if(tag >= errors->size())
        {
            errors->resize(tag+1);
        }

        (*errors)[tag] += error;
    }

    /*
     * += errorComposants
     */
    void addErrors(const ErrorSum& errorSum2)
    {
        addMap(*errorSum2.errors, [](errorType e){return e;});
    }

    /*
     * -= errorComposants
     */
    void subErrors(const ErrorSum& errorSum2)
    {
        addMap(*errorSum2.errors, [](errorType e){return -e;});
    }

    /*
     * += scalar * errorComposants
     */
    void addErrorsTimeScalar(const ErrorSum& errorSum2, errorType scalar)
    {
        addMap(*errorSum2.errors, [scalar](errorType e){return e*scalar;});
    }

    /*
     * -= scalar * errorComposants
     */
    void subErrorsTimeScalar(const ErrorSum& errorSum2, errorType scalar)
    {
        addMap(*errorSum2.errors, [scalar](errorType e){return -e*scalar;});
    }

    //-------------------------------------------------------------------------
    // HIGH LEVEL FUNCTIONS

    /*
     * applies a function f to the elements of errors2 and add them to errors
     */
    template<typename FUN>
    inline void addMap(const std::vector<errorType>& errors2, FUN f)
    {
        // insures that errors is big enough to store the results
        if(errors2.size() > errors->size())
        {
            errors->resize(errors2.size());
        }

        // adds the values from errors2 to errors
        for(int tag = 0; tag < errors2.size(); tag++)
        {
            (*errors)[tag] += f(errors2[tag]);
        }
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
        for(auto error : *errors)
        {
            totalAbsoluteError += std::abs(error);
            totalError += error;
        }
        // rectifies the sign of the sum of absolute errors to match the sign of the sum
        if(totalError < 0)
        {
            totalAbsoluteError = -totalAbsoluteError;
        }

        // collects the relevant data expressed in percent of the total error
        std::vector<std::pair<Tag, int>> data;
        bool droppedNonSignificantTerms = false;
        for(int tag = 0; tag < errors->size(); tag++)
        {
            errorType error = (*errors)[tag];
            int percent = (error*100.) / totalAbsoluteError;

            if(std::abs(percent) >= minErrorPercent)
            {
                data.push_back(std::make_pair(tag, percent));
            }
            else if (error != 0)
            {
                droppedNonSignificantTerms = true;
            }
        }

        // sorts the vector by abs(error) descending
        auto compare = [](const std::pair<Tag, errorType>& p1, const std::pair<Tag, errorType>& p2){return std::abs(p1.second) > std::abs(p2.second);};
        std::sort(data.begin(), data.end(), compare);

        // output stream
        std::ostringstream output;
        output << '[';

        // displays each other element prefixed by a ", " separator
        if(data.size() > 0)
        {
            auto kv = data[0];
            output << Block::nameOfTag(kv.first) << ':' << kv.second << '%';

            for(int i = 1; i < data.size(); i++)
            {
                kv = data[i];
                output << ", " << Block::nameOfTag(kv.first) << ':' << kv.second << '%';
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

#endif //SHAMAN_ERRORSUM_H
