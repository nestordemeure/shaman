//
// Created by chevalier on 08/08/18.
//

#include <ostream>

#include <shaman.h>

#include "utils.h"

using namespace Sstd;

//---------------------------------------------------------------------------------------
// CADNA FAILURES

Sdouble squaredTriangleArea(Sdouble x, Sdouble y, Sdouble z)
{
  return (x + y + z) * (z - (x - y)) * (z + (x - y)) * (x + (y - z));
}

/*
 * cadna find 0 significative digits here, a false positive (identified by kahan)
 *
 * it seems that it was due to a bug in cadna since corrected
 */
void falsePositive()
{
  std::cout << "False positive." << '\n'
            << "Example of a problem where CADNA is said to falsly detects that the result has no significative digits"
            << std::endl;

  Sdouble x = 1.234568e6;
  Sdouble y = 1.234567e6;
  Sdouble z = 1.00000000023;
  Sdouble r = sqrt( squaredTriangleArea(x,y,z) / squaredTriangleArea(x,y,2.0*z) );

  std::cout << "ratio of areas : " << r << " (exact result  1.238278374e-05)" << std::endl;

  displayError(r, 1.238278374e-05);
}

//-----

/*
 * an example where cadna overestimate the number of significative digits
 */
void alternatingCounter()
{
  std::cout << "An example founc in verificarlo's main schrodinger." << '\n'
            << "CADNA overestimate the number of significative digits" << std::endl;

  Sdouble c = -5e13;

  for(unsigned int i = 0; i < 100000000; i++)
  {
    if (i%2 == 0)
    {
      c = c + 1.e6;
    }
    else
    {
      c = c - 1.e-6;
    }
  }

  std::cout << "result=" << c << " expected result=" << -50 << std::endl;
  displayError(c, -50);
}

//-----

/*
 * demonstration of error followed through a trigonometric function
 *
 * useful to showcase src's ability to follow error in transcendentalfunctions while others (Verrou, Cadna?, verificarlo?) just ignore them
 */
void trigoTest()
{
  std::cout << "An example, using a trigonometric function, where CADNA overestimate the number of significative digits." << std::endl;

  /*
   * 2e20 : no significativ digits, everybody is correct
   * 2e25 : there is one significative digit (pure luck),  src might detect it
   * 2e30 : no significative digits, cadna is optimistic by 2 digits, src is correct
   */
  double bigNum = 2e30;
  double epsi = 10;
  double result = cos(epsi);

  // double
  {
    double x = bigNum;
    double y = x + epsi;

    double sum1 = cos(x-y);
    double sum2 = cos(x)*cos(y) + sin(x)*sin(y);
    std::cout << "DOUBLE cos(x-y) = " << sum1 << " = " << sum2 << " (true result=" << result << ')' << std::endl;
  }

#ifdef CADNA
  // cadna
  {
    Cdouble x = bigNum;
    Cdouble y = x + epsi;

    Cdouble sum1 = cos(x-y);
    Cdouble sum2 = cos(x)*cos(y) + sin(x)*sin(y);
    std::cout << "CADNA  cos(x-y) = " << sum1 << " = " << sum2 << " (true result=" << result << ')' << std::endl;
  }
#endif

  // src
  {
    Sdouble x = bigNum;
    Sdouble y = x + epsi;

    Sdouble sum1 = cos(x-y);
    Sdouble sum2 = cos(x)*cos(y) + sin(x)*sin(y);
    std::cout << "SHAMAN cos(x-y) = " << sum1 << " = " << sum2 << " (true result=" << result << " estimated-true-result=" << (sum1.number+sum1.error) << ')' << std::endl;
  }
}


int main()
{
  falsePositive();
  alternatingCounter();
  trigoTest();

  return EXIT_SUCCESS;
}