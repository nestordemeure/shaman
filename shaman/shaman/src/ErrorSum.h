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
    // uses a pointer to vector to keep type size constant TODO has this indirection any direct advantages ?
    // sorted from bigger tag to smaller tag in the hope of speeding up map2 and insertion
    std::vector<errorType> errors;
    std::vector<Tag> tags;
    // TODO a true sparse vector implementation might have better performances

    //-------------------------------------------------------------------------
    // CONSTRUCTORS

    /*
     * empty constructor : currently no error
     */
    explicit ErrorSum(): errors(), tags() {}

    /*
     * copy constructor
     * WARNING this constructor needs to do a deep copy (which is not the default)
     */
    ErrorSum(const ErrorSum& errorSum2): errors(errorSum2.errors), tags(errorSum2.tags) {}

    /*
     * copy assignment
     * WARNING this constructor needs to do a deep copy (which is not the default)
     */
    ErrorSum& operator=(const ErrorSum& errorSum2)
    {
        errors = errorSum2.errors;
        tags = errorSum2.tags;
        return *this;
    };

    /*
     * returns an errorSum with a single element (singleton)
     */
    explicit ErrorSum(Tag tag, errorType error): errors(), tags()
    {
        if(error != 0)
        {
            errors.push_back(error);
            tags.push_back(tag);
        }
    }

    /*
     * returns an errorSum with a single element (singleton)
     * uses the current tag
     */
    explicit ErrorSum(errorType error): errors(), tags()
    {
        if(error != 0)
        {
            Tag tag = Block::currentBlock();
            errors.push_back(error);
            tags.push_back(tag);
        }
    }

    //-------------------------------------------------------------------------
    // OPERATIONS

    /*
     * ~-
     */
    void unaryNeg()
    {
        for(auto& error : errors)
        {
            error = -error;
        }
    }

    /*
     * *= scalar
     */
    void multByScalar(errorType scalar)
    {
        for(auto& error : errors)
        {
            error *= scalar;
        }
    }

    /*
     * /= scalar
     */
    void divByScalar(errorType scalar)
    {
        for(auto& error : errors)
        {
            error /= scalar;
        }
    }

    /*
     * += error
     * TODO might use binary search to improve perfs when vector is big
     */
    void addError(errorType error)
    {
        Tag tag = Block::currentBlock();

        // search in a sorted vector
        for(int i = 0; i < tags.size(); i++)
        {
            auto& currentTag = tags[i];
            if(currentTag < tag)
            {
                // the target tag is not inside the vector but should be here
                errors.insert(errors.begin()+i, error);
                tags.insert(tags.begin()+i, tag);
                return;
            }
            else if (currentTag == tag)
            {
                // we found the target tag
                errors[i] += error;
                return;
            }
        }

        // the target was not in the vector
        errors.push_back(error);
        tags.push_back(tag);
    }

    /*
     * += errorComposants
     */
    void addErrors(const ErrorSum& errors2)
    {
        addMap(errors2, [](errorType e){return e;});
    }

    /*
     * -= errorComposants
     */
    void subErrors(const ErrorSum& errors2)
    {
        addMap(errors2, [](errorType e){return -e;});
    }

    /*
     * += scalar * errorComposants
     */
    void addErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        addMap(errors2, [scalar](errorType e){return e*scalar;});
    }

    /*
     * -= scalar * errorComposants
     */
    void subErrorsTimeScalar(const ErrorSum& errors2, errorType scalar)
    {
        addMap(errors2, [scalar](errorType e){return -e*scalar;});
    }

    //-------------------------------------------------------------------------

    /*
     * applies a function f to the elements of errors2 and add them to errors
     * TODO we might use a std algorithm to do several insertions at once in o(n)
     * TODO reducing indirections might improve perfs
     */
    template<typename FUN>
    inline void addMap(const ErrorSum& errorSum2, FUN f)
    {
        auto& errors2 = errorSum2.errors;
        auto& tags2 = errorSum2.tags;

        // iterate on it1 and it2 at the same time
        int i1 = 0;
        int i2 = 0;
        while( (i1 < errors.size()) && (i2 < errors2.size()) )
        {
            if(tags2[i2] == (tags)[i1])
            {
                (errors)[i1] += f(errors2[i2]);
                i2++;
            }
            else if (tags2[i2] > (tags)[i1])
            {
                errors.insert(errors.begin()+i1, f(errors2[i2]));
                tags.insert(tags.begin()+i1, tags2[i2]);
                i2++;
            }
            i1++;
        }

        // we did not get to the end of it2
        if(i2 < errors2.size())
        {
            auto errorsPreviousSize = errors.size();
            tags.insert(tags.end(), tags2.begin()+i2, tags2.end());
            errors.insert(errors.end(), errors2.begin()+i2, errors2.end());
            partialTransform((errors), errorsPreviousSize, errors.size(), f);
        }
        /*
        // TODO test wether this is faster
        while(i2 < errors2.size())
        {
            errors->push_back(f(errors2[i2]));
            tags->push_back(tags2[i2]);
            i2++;
        }
        */
    }

    /*
     * applies a fucntion to a portion of an array
     */
    template<typename FUN>
    inline void partialTransform(std::vector<errorType>& vect , unsigned long inf, unsigned long sup, FUN f)
    {
        for(unsigned long i = inf; i < sup; i++)
        {
            vect[i] = f(vect[i]);
        }
    }

    //-------------------------------------------------------------------------
    // DISPLAY

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

        if(errors.empty())
        {
            output << "no-error";
        }
        else
        {
            // copy the data
            std::vector<std::pair<Tag, errorType>> data;
            for(int i = 0; i < errors.size(); i++)
            {
                data.push_back(std::make_pair(tags[i], errors[i]));
            }

            // sorts the vector by abs(error) descending
            auto compare = [](const std::pair<Tag, errorType>& p1, const std::pair<Tag, errorType>& p2){return std::abs(p1.second) > std::abs(p2.second);};
            std::sort(data.begin(), data.end(), compare);

            // functions that returns only if the error is big enough to have an impact given the type
            int nbDigitsMax = std::numeric_limits<errorType>::digits10 + 1;
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
};

#endif //SHAMAN_ERRORSUM_H
