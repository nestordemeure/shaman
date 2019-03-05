//
// Created by demeuren on 04/06/18.
//
#pragma once

#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "tagger.h"
#include "memory_store.h"

template<typename errorType> class error_sum
{
public:
    // contains the error decomposed in composants (one per block encountered)
    // errors[tag] = error // if tag is out or range, error is 0
    std::vector<errorType>* errors;
    const static int CONSTRUCTOR_FLAG = 32771; // a number that is unlikely to appear at random in unalocated memory
    int constructor_flag = CONSTRUCTOR_FLAG; // used to test wether an error_sum was constructed legally

    //-------------------------------------------------------------------------
    // CONSTRUCTORS

    /*
     * empty constructor : currently no error
     */
    explicit error_sum(): errors(MemoryStore<errorType>::getVector()) {}

    /*
     * copy constructor
     * WARNING this constructor needs to do a deep copy (which is not the default)
     * DANGER removing this constructor (since the template constructor should be enough to cover even the base errorType) will result in a memory leak
     */
    error_sum(const error_sum& errorSum2): errors(MemoryStore<errorType>::getVector())
    {
        errors->assign(errorSum2.errors->begin(), errorSum2.errors->end()); // assign insures that we do not drop the capacity (undefined behaviour for = operator)
    }

    /*
     * copy constructor that allows construction from another errorType
     */
    template<typename errorType2>
    error_sum(const error_sum<errorType2>& errorSum2): errors(MemoryStore<errorType>::getVector())
    {
        errors->assign(errorSum2.errors->begin(), errorSum2.errors->end());
    }

    /*
     * copy assignment
     * WARNING this constructor needs to do a deep copy (which is not the default)
     */
    error_sum& operator=(const error_sum& errorSum2)
    {
        // if the memory comes from an uninitialized allocation
        if (constructor_flag != CONSTRUCTOR_FLAG)
        {
            errors = MemoryStore<errorType>::getVector();
            constructor_flag = CONSTRUCTOR_FLAG;
        }
        *errors = *(errorSum2.errors); // cannot use 'assign' because errors might have existing values out of errorSum2.size()
        return *this;
    };

    /*
     * returns an errorSum with a single element (singleton)
     */
    explicit error_sum(Tag tag, errorType error): errors(MemoryStore<errorType>::getVector())
    {
        errors->resize(tag+1);
        (*errors)[tag] = error;
    }

    /*
     * returns an errorSum with a single element (singleton)
     * uses the current tag
     */
    explicit error_sum(errorType error): errors(MemoryStore<errorType>::getVector())
    {
        Tag tag = CodeBlock::currentBlock();
        errors->resize(tag+1);
        (*errors)[tag] = error;
    }

    /*
     * destructor
     */
    ~error_sum()
    {
        MemoryStore<errorType>::releaseVector(errors);
        errors = nullptr;
        constructor_flag = 0;
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
        Tag tag = CodeBlock::currentBlock();

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
    void addErrors(const error_sum& errorSum2)
    {
        addMap(*errorSum2.errors, [](errorType e){return e;});
    }

    /*
     * -= errorComposants
     */
    void subErrors(const error_sum& errorSum2)
    {
        addMap(*errorSum2.errors, [](errorType e){return -e;});
    }

    /*
     * += scalar * errorComposants
     */
    void addErrorsTimeScalar(const error_sum& errorSum2, errorType scalar)
    {
        addMap(*errorSum2.errors, [scalar](errorType e){return e*scalar;});
    }

    /*
     * -= scalar * errorComposants
     */
    void subErrorsTimeScalar(const error_sum& errorSum2, errorType scalar)
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
            totalAbsoluteError += std::fabs(error);
            totalError += error;
        }
        // rectifies the sign of the sum of absolute errors to match the sign of the sum
        if(totalError < 0)
        {
            totalAbsoluteError = -totalAbsoluteError;
        }

        // collects the relevant data expressed in percent of the total error
        std::vector<std::pair<Tag, errorType>> data;
        bool droppedNonSignificantTerms = false;
        for(int tag = 0; tag < errors->size(); tag++)
        {
            errorType error = (*errors)[tag];

            if (std::isnan(error))
            {
                data.push_back(std::make_pair(tag, error));
            }
            else
            {
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


