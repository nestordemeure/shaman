//
// Created by demeuren on 02/05/18.
//

#ifndef SHAMAN_SIGNIFICATIVITY_H
#define SHAMAN_SIGNIFICATIVITY_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

#include <shaman/tagged/GlobalVar.h>

//-----------------------------------------------------------------------------
// HIGH PRECISION APPROXIMATION

/*
 * tries to return a better approximation of the number by taking the numerical error into account
 */
templated inline preciseType Snum::corrected_number() const
{
    return preciseType(number) + preciseType(error);
}

//-----------------------------------------------------------------------------
// SIGNIFICATIV DIGITS

/*
 * returns the number of (relative) significative digits of a couple (number,error)
 *
 * the exact 0 is handled by a special case (otherwise it cannot have any significativ digits)
 */
templated inline numberType Snum::digits(numberType number, errorType error)
{
    if (error == 0)
    {
        // no error -> theorically infinite precision
        return INFINITY;
    }
    else if (number == 0)
    {
        // we count the number of significant zeroes
        return std::max(0., -log10(std::abs(error)) - 1);
    }
    else
    {
        numberType relativeError = std::abs((numberType) error / number);

        if (relativeError >= 1)
        {
            return 0;
        }
        else
        {
            return -std::log10(relativeError);
        }
    }
}

/*
 * returns the number of significative digits of a Snum
 */
templated inline numberType Snum::digits() const
{
    return digits(number, error);
}

//-----------------------------------------------------------------------------
// SIGNIFICATIVITY TEST

/*
 * returns true if the couple (number,error) has no significant digits in the base
 *
 * NOTE : 'error != 0' is optional
 * it slightly improves the performances on some test cases
 * since it is common to have numbers with 0 error
 * (any number that just been turned into a S)
 */
templated inline bool Snum::non_significant(numberType number, errorType error)
{
    int base = 10;
    return (error != 0) && (std::abs(number) < base * std::abs(error));
}

/*
 * returns true if the current S has no significative digits in the base
 */
templated inline bool Snum::non_significant() const
{
    return non_significant(number, error);
}

/*
 * function called at each unstability
 * put a breakpoint here to break at each unstable tests
 *
 * TODO we could use the block name as a form of stack trace to locate the unstability
 */
void Shaman::unstability()
{
    ShamanGlobals::unstableBranchCounter++;
}

/*
 * check wether a branch is unstable
 * in wihci case it triggers the unstability function
 */
templated inline void Snum::checkUnstableBranch(Snum n1, Snum n2)
{
    bool isUnstable = non_significant(n1.number - n2.number, n1.error - n2.error);
    if(isUnstable)
    {
        Shaman::unstability();
    }
}

/*
 * displays the number of unstable branches
 */
inline void Shaman::displayUnstableBranches()
{
    std::cout << "#SHAMAN: " << "We detected " << ShamanGlobals::unstableBranchCounter << " unstable tests." << std::endl;
}

//-----------------------------------------------------------------------------
// STRING CONVERSIONS

/*
 * streaming operator, displays only the significative digits
 * (alternative version : outputs the number, the error and the number of significative digits)
 */
templated inline std::ostream& operator<<(std::ostream& os, const Snum& n)
{
    int nbDigitsMax = std::numeric_limits<numberType>::digits10 + 2; // since this is a maximum, we add two to avoid being too pessimistic (17 for double)
    numberType fdigits = std::floor(n.digits());

    if (std::isnan(n.number)) // not a number
    {
        os << "nan";
    }
    else if (fdigits <= 0) // no significant digits
    {
        // the first zeros might be significant
        int digits = std::floor(Snum::digits(0, n.error));

        if ((std::abs(n.number) >= 1) || (digits <= 0))
        {
            // the number has no meaning
            os << "~numerical-noise~";
        }
        else
        {
            // some zeros are significant
            digits = std::min(nbDigitsMax, digits);
            os << std::scientific << std::setprecision(digits-1) << 0.0; // TODO should we add a prefix to indicate that the number is non significant beyond the zeros ?
        }
    }
    else // a perfectly fine number
    {
        int digits = std::min((numberType) nbDigitsMax, fdigits);
        os << std::scientific << std::setprecision(digits-1) << n.number;
    }

    os << ' ' << (std::string) n.errorComposants;

    return os;
}

/*
 * extraction operator to cast a stream into an Snum
 */
templated std::istream& operator>>(std::istream& is, Snum& n)
{
    // extracts the number
    numberType num;
    is >> num;

    // modifies the Snum in place
    n.number = num;
    n.errorComposants = Serror(0);

    return is;
}

/*
 * method to convert a Snum into a string
 *
 * using the streaming operator
 * there is probably a more efficient implementation but it was the easiest way to benefit from std::scientific and std::setprecision
 */
templated std::string Snum::to_string() const
{
    std::ostringstream stream;
    stream << this;
    return stream.str();
}

/*
 * function to convert a Snum into a string
 *
 * NOTE : code duplicated from .to_string()
 */
templated inline std::string Sstd::to_string(const Snum& n)
{
    std::ostringstream stream;
    stream << n;
    return stream.str();
};

/*
 * convert a value into a C string (const char *)
 */
template<typename T>
inline const char* Sstd::to_Cstring(const T &n)
{
    return Sstd::to_string(n).c_str();
};

//-----------------------------------------------------------------------------
#endif //SHAMAN_SIGNIFICATIVITY_H
