//
// taken from :
// https://martin-thoma.com/fractions-in-cpp/
//

#ifndef SHAMAN_DEMO_FRACTION_H
#define SHAMAN_DEMO_FRACTION_H

#include <iostream>

using namespace std;

class Fraction
{
private:
    // Calculates the greates common divisor with
    // Euclid's algorithm
    // both arguments have to be positive
    long long gcd(long long a, long long b)
    {
        while (a != b)
        {
            if (a > b)
            {
                a -= b;
            }
            else
            {
                b -= a;
            }
        }
        return a;
    }

public:
    long long numerator, denominator;

    Fraction()
    {
        numerator = 0;
        denominator = 1;
    }

    Fraction(long long n)
    {
        numerator = n;
        denominator = 1;
    }

    Fraction(long long n, long long d)
    {
        if (d == 0)
        {
            cerr << "Denominator may not be 0." << endl;
            exit(0);
        }
        else if (n == 0)
        {
            numerator = 0;
            denominator = 1;
        }
        else
        {
            int sign = 1;
            if (n < 0)
            {
                sign *= -1;
                n *= -1;
            }
            if (d < 0)
            {
                sign *= -1;
                d *= -1;
            }

            long long tmp = gcd(n, d);
            numerator = n / tmp * sign;
            denominator = d / tmp;
        }
    }

    operator int() { return (numerator) / denominator; }
    operator float() { return ((float) numerator) / denominator; }
    operator double() { return ((double) numerator) / denominator; }
};

inline Fraction operator+(const Fraction &lhs, const Fraction &rhs)
{
    Fraction tmp(lhs.numerator * rhs.denominator
                 + rhs.numerator * lhs.denominator,
                 lhs.denominator * rhs.denominator);
    return tmp;
}

inline Fraction operator+=(Fraction &lhs, const Fraction &rhs)
{
    Fraction tmp(lhs.numerator * rhs.denominator
                 + rhs.numerator * lhs.denominator,
                 lhs.denominator * rhs.denominator);
    lhs = tmp;
    return lhs;
}

inline Fraction operator-(const Fraction &lhs, const Fraction &rhs)
{
    Fraction tmp(lhs.numerator * rhs.denominator
                 - rhs.numerator * lhs.denominator,
                 lhs.denominator * rhs.denominator);
    return tmp;
}

inline Fraction operator-=(Fraction &lhs, const Fraction &rhs)
{
    Fraction tmp(lhs.numerator * rhs.denominator
                 - rhs.numerator * lhs.denominator,
                 lhs.denominator * rhs.denominator);
    lhs = tmp;
    return lhs;
}

inline Fraction operator*(const Fraction &lhs, const Fraction &rhs)
{
    Fraction tmp(lhs.numerator * rhs.numerator,
                 lhs.denominator * rhs.denominator);
    return tmp;
}

inline Fraction operator*=(Fraction &lhs, const Fraction &rhs)
{
    Fraction tmp(lhs.numerator * rhs.numerator,
                 lhs.denominator * rhs.denominator);
    lhs = tmp;
    return lhs;
}

inline Fraction operator*(int lhs, const Fraction &rhs)
{
    Fraction tmp(lhs * rhs.numerator, rhs.denominator);
    return tmp;
}

inline Fraction operator*(const Fraction &rhs, int lhs)
{
    Fraction tmp(lhs * rhs.numerator, rhs.denominator);
    return tmp;
}

inline Fraction operator/(const Fraction &lhs, const Fraction &rhs)
{
    Fraction tmp(lhs.numerator * rhs.denominator,
                 lhs.denominator * rhs.numerator);
    return tmp;
}

inline std::ostream &operator<<(std::ostream &strm, const Fraction &a)
{
    if (a.denominator == 1)
    {
        strm << a.numerator;
    }
    else
    {
        strm << a.numerator << "/" << a.denominator;
    }
    return strm;
}

#endif //SHAMAN_DEMO_FRACTION_H
