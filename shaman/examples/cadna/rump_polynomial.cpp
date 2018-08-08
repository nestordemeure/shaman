//
// Created by chevalier on 08/08/18.
//

/*
 * Examples inspired by Cadna numerical examples page :
 * http://cadna.lip6.fr/Examples_Dir/Accueil.php
 */

#include <ostream>

#include <shaman.h>

//---------------------------------------------------------------------------------------
// POLYNOMIAL

Sdouble P(Sdouble x, Sdouble y)
{
  return 9.0*x*x*x*x - y*y*y*y + 2.0*y*y;
}

/*
 * a polynomial proposed by RUMP
 *
 * the first result is bad, the second is perfect
 * can we differentiate them ?
 *
 * One can see the difference of accuracy between the two results even if they were provided with the same algorithm.
 * It points out the importance of the data influence on the numerical quality of results given by an algorithm.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex1.php
 */
void polynomial()
{
  std::cout << "Rump equation." << '\n'
            << "Can we differentiate a bad result from a good one ?" << std::endl;

  Sdouble x = 10864.0;
  Sdouble y = 18817.0;
  Sdouble res = P(x,y);
  std::cout << "P(10864,18817) = " << res << " (exact value : 1)" << std::endl;
  // displayError(res, 1);

  x = 1.0 / 3.0;
  y = 2.0 / 3.0;
  res = P(x,y);
  std::cout << "P(1/3,2/3) = " << res << std::endl;
  //displayError(P(x,y), 0.8024691358024691); // best possible according to CADNA

  std::cout << std::endl;
}

int main() {
  polynomial();
  
  return EXIT_SUCCESS;
}