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

template<typename errorType> class ErrorSum
{
public:
    // contains the error decomposed in composants (one per block encountered)
    using sparseVec = std::vector<std::pair<Tag,errorType>>; // TODO a true sparse vector implementation might have better performances
    using sparseVec_ptr = std::unique_ptr<sparseVec>; // pointer to vector to keep type size constant
    sparseVec_ptr errors; // sorted from bigger tag to smaller tag in the hope of speeding up map2 and insertion

    //-------------------------------------------------------------------------

    /*
     * empty constructor : currently no error
     */
    explicit ErrorSum(): errors(new std::vector<std::pair<Tag,errorType>>()) {}

    /*
     * copy constructor
     * WARNING this constructor needs to do a deep copy (which is not the default)
     */
    ErrorSum(const ErrorSum& errorSum2): errors(new std::vector<std::pair<Tag,errorType>>(*(errorSum2.errors))) {}

    /*
     * copy assignment
     * WARNING this constructor needs to do a deep copy (which is not the default)
     */
    ErrorSum& operator=(const ErrorSum& errorSum2)
    {
        errors.reset(new std::vector<std::pair<Tag,errorType>>(*(errorSum2.errors)));
        return *this;
    };

    /*
     * returns an errorSum with a single element (singleton)
     */
    explicit ErrorSum(Tag tag, errorType error): errors(new std::vector<std::pair<Tag,errorType>>())
    {
        if(error != 0)
        {
            errors->push_back(std::make_pair(tag,error));
        }
    }

    /*
     * returns an errorSum with a single element (singleton)
     * uses the current tag
     */
    explicit ErrorSum(errorType error): errors(new std::vector<std::pair<Tag,errorType>>())
    {
        if(error != 0)
        {
            Tag tag = Block::currentBlock();
            errors->push_back(std::make_pair(tag,error));
        }
    }

    /*
     * produces a readable string representation of the error terms
     * the error terms are sorted from larger to smaller
     * displays only the maxElementNumberDisplayed first elements with an amplitude big enough to matter given the type (10^-19 doesn't matter for a float
     */
    explicit operator std::string() const
    {
        std::ostringstream output;
        output << std::scientific << std::setprecision(2) << '[';
        int maxElementNumberDisplayed = 5;

        if(errors->empty())
        {
            output << "no-error";
        }
        else
        {
            // copy the data
            sparseVec data(*errors);

            // sorts the vector by abs(error) descending
            auto compare = [](const std::pair<Tag, errorType>& p1, const std::pair<Tag, errorType>& p2){return std::abs(p1.second) > std::abs(p2.second);};
            std::sort(data.begin(), data.end(), compare);

            // functions that returns only if the error is big enough to have an impact given the type
            int nbDigitsMax = std::numeric_limits<errorType>::digits10;
            auto significant = [nbDigitsMax](const std::pair<Tag, errorType>& p){return -std::log10(std::abs(p.second)) < nbDigitsMax;};

            // displays the first element
            int i = 0;
            auto kv = data[0];
            output << Block::nameOfTag(kv.first) << ':' << kv.second;
            i++;

            // displays each other element prefixed by a ", " separator
            while((i < maxElementNumberDisplayed) && (i < data.size()) && (significant(data[i])))
            {
                kv = data[i];
                output << ", " << Block::nameOfTag(kv.first) << ':' << kv.second;
                i++;
            }

            // adds a reminder that we are not displaying all error terms
            if(i < data.size())
            {
                output << "…";
            }
        }

        output << ']';
        return output.str();
    }

    //-------------------------------------------------------------------------

    /*
     * ~-
     */
    void unaryNeg()
    {
        transformErrors([](errorType e){ return -e; });
    }

    /*
     * *= scalar
     */
    void multByScalar(errorType scalar)
    {
        transformErrors([scalar](errorType e){ return e * scalar; });
    }

    /*
     * /= scalar
     */
    void divByScalar(errorType scalar)
    {
        transformErrors([scalar](errorType e){ return e / scalar; });
    }

    /*
     * += error
     * TODO might use binary search to improve perfs
     */
    void addError(errorType error)
    {
        Tag tag = Block::currentBlock();

        // search in a sorted vector
        for(int i = 0; i < errors->size(); i++)
        {
            Tag currentTag = errors->at(i).first;
            if(currentTag < tag)
            {
                // the target tag is not inside the vector but should be here
                auto kv2 = std::make_pair(tag,error);
                errors->insert(errors->begin()+i, kv2);
                return;
            }
            else if (currentTag == tag)
            {
                // we found the target tag
                errors->at(i).second += error;
                return;
            }
        }

        // the target was not in the vector
        errors->push_back(std::make_pair(tag,error));
    }

    /*
     * += errorComposants
     */
    void addErrors(const ErrorSum& errors2)
    {
        addMap(errors2.errors, [](errorType e){return e;});
    }

    /*
     * -= errorComposants
     */
    void subErrors(const ErrorSum& errors2)
    {
        addMap(errors2.errors, [](errorType e){return -e;});
    }

    /*
     * += scalar * errorComposants
     */
    void addErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        addMap(errors2.errors, [scalar](errorType e){return e*scalar;});
    }

    /*
     * -= scalar * errorComposants
     */
    void subErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        addMap(errors2.errors, [scalar](errorType e){return -e*scalar;});
    }

    //-------------------------------------------------------------------------

    /*
     * compute the sum of errors
     * NOTE : useful for debug
     */
    inline errorType sumErrors() const
    {
        errorType result = 0.;
        for(auto& kv : *errors)
        {
            result += kv.second;
        }
        return result;
    }

    /*
     * homemade implementation of std::transform since I was unable to use the original
     */
    template<typename FUN>
    inline void transformErrors(FUN f)
    {
        for(auto& kv : *errors)
        {
            kv.second = f(kv.second);
        }
    }

    /*
     * applies a function f to the elements of errors2 and add them to errors
     */
    template<typename FUN>
    inline void addMap(const sparseVec_ptr& errors2, FUN f)
    {
        // iterate on it1 and it2 at the same time
        int i1 = 0;
        int i2 = 0;
        while( (i1 < errors->size()) && (i2 < errors2->size()) )
        {
            auto& kv1 = errors->at(i1);
            auto& kv2 = errors2->at(i2);
            if(kv1.first == kv2.first)
            {
                kv1.second += f(kv2.second);
                i2++;
            }
            else if (kv1.first < kv2.first)
            {
                auto kv = std::make_pair(kv2.first, f(kv2.second));
                errors->insert(errors->begin()+i1, kv);
                i2++;
            }
            i1++;
        }

        // we did not get to the end of it2
        while(i2 < errors2->size())
        {
            auto& kv2 = errors2->at(i2);
            auto kv = std::make_pair(kv2.first, f(kv2.second));
            errors->push_back(kv);
            i2++;
        }
    }
};

#endif //SHAMAN_ERRORSUM_H
