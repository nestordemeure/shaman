//
// Created by nestor on 02/02/18.
//

#ifndef VARIABLE_DECL_H
#define VARIABLE_DECL_H

#include "Variable.h"

//-----------------------------------------------------------------------------
// CONSTRUCTORS

/*
* takes a name and produces a Snumber with the given name
*/
Variable::Variable (std::string name) : Snumber()
{
    variableName = std::move(name);
    maximumRelativeError = 0;
};

/*
 * constructor that contains debug information
 *
 * used in the macro definition
 */
Variable::Variable(std::string name, std::string file, int line) : Snumber()
{
    variableName = std::move(name);
    fileName = std::move(file);
    lineNumber = std::move(line);
    maximumRelativeError = 0;
}

/*
 * takes a name and a number and produces a Snumber with 0 error and a given name
 */
Variable::Variable(std::string name, numberType numberArg) : Snumber(numberArg)
{
    variableName = std::move(name);
    maximumRelativeError = 0;
}

/*
 * takes a name and a Snumber and produces a Snumber with the given name
 */
Variable::Variable(std::string name, Snumber n) : Snumber(n)
{
    variableName = std::move(name);
    updateMaxRelativeError();
}

//-----------------------------------------------------------------------------
// OBJECT MANIPULATION

/*
 * assign n to the current Variable
 */
Variable& Variable::operator=(const Snumber& n)
{
    number = n.number;
    error = n.error;
    updateMaxRelativeError();

    return *this;
}

/*
 * assign v to the current variable
 *
 * does not change the current variableName
 */
Variable& Variable::operator=(const Variable& v)
{
    number = v.number;
    error = v.error;
    maximumRelativeError = std::max(maximumRelativeError, v.maximumRelativeError);

    return *this;
}

/*
 * destructor
 *
 * used to display information about named variables as they go out of scope
 */
Variable::~Variable()
{
    double relativeError = std::abs((double) error/number);
    double digits = std::max(0.0, -log10(relativeError));
    double minimumDigits = std::max(0.0, -log10((double) maximumRelativeError));

    // displays variable name
    std::cout << '#' << variableName << '\t';

    // displays position information if available
    if (!fileName.empty())
    {
        std::cout << "file:" << fileName << '\t' << "line:" << lineNumber << '\n' << '\t';
    }

    // displays error
    std::cout << std::scientific
              << "absolute_error=" << error << '\t'
              << std::fixed << std::setprecision(2)
              << "relative_error=" << (relativeError*100) << '%' << '\t'
              << "max_observed_relative_error=" << (maximumRelativeError*100) << '%' << '\n' << '\t'
              << "significative_digits=" << digits << '\t'
              << "min_observed_significative_digits=" << minimumDigits << std::endl;
}

/*
 * update the current maximum relative error
 */
void Variable::updateMaxRelativeError()
{
    if (std::abs(error) > std::abs(number * maximumRelativeError))
    {
        maximumRelativeError = std::abs(error/number);
    }
}

//-----------------------------------------------------------------------------
// OPERATORS

// +=
Variable& Variable::operator+=(const Snumber& n)
{
    *this = (*this) + n;

    return *this;
}

// -=
Variable& Variable::operator-=(const Snumber& n)
{
    *this = (*this) - n;

    return *this;
}

// *=
Variable& Variable::operator*=(const Snumber& n)
{
    *this = (*this) * n;

    return *this;
}

// /=
Variable& Variable::operator/=(const Snumber& n)
{
    *this = (*this) / n;

    return *this;
}

#endif //VARIABLE_DECL_H