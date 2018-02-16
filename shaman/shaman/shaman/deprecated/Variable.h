//
// Created by demeuren on 01/02/18.
//

#ifndef COMPENSATIONS_VARIABLE_H
#define COMPENSATIONS_VARIABLE_H

#include "Shaman.h"

/*
 * represents a number (Snumber) with a name (a variable)
 * it keeps a tab on the maximum relative error and output the informations on destruction
 */
class Variable: public Snumber
{
public :
    // variable description
    std::string variableName;
    std::string fileName;
    int lineNumber;
    // history of the variable
    errorType maximumRelativeError; // maximum relative error observed so far

    // constructors
    Variable (std::string name);
    Variable(std::string name, std::string file, int line);
    Variable (std::string name, numberType number);
    Variable (std::string name, Snumber number);

    // object manipulation
    Variable& operator=(const Snumber& n);  // assignment
    Variable& operator=(const Variable& v);  // assignment
    ~Variable();   // destructor (used to display information)
    void updateMaxRelativeError();

    // class operators
    Variable& operator+=(const Snumber& n);
    Variable& operator-=(const Snumber& n);
    Variable& operator*=(const Snumber& n);
    Variable& operator/=(const Snumber& n);
};

/*
 * shortcut to create variables (plus debug informations)
 *
 * declares a variable in two steps so that
 * - 1 - the variable is properly created and assigned
 * - 2 - if some value was to be put into the variable, it will be
 *
 * avoiding "; name=name" would be nice
 * but it is required to deal with variables that take a value on creation
 * deleting it turns "V var = x;" into "V var = (new original_var = x);"
 * causing original var to be deleted at the end of the line (trigerring our destructor)
 */
#define V(name) Variable name = Variable(#name, __FILE__, __LINE__); name = name

#endif //COMPENSATIONS_VARIABLE_H
#include "Variable_Decl.h"