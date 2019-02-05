//
// Created by chevalier on 08/08/18.
//

#include <ostream>
#include <shaman.h>

using Sstd::sqrt;

//---------------------------------------------------------------------------------------
// SECOND ORDER EQUATION

/*
 * The standard floating point arithmetic cannot detect that d=0. The wrong branching is performed and the result is false.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex2.php
 */
void secondOrder()
{
  std::cout << "Second order equation." << '\n'
            << "Discriminant d=0, x1=x2=3.5" << std::endl;

  Sdouble a = 0.3;
  Sdouble b = -2.1;
  Sdouble c = 3.675;

  if (a == 0.0)
  {
    if (b == 0.0)
    {
      if (c == 0.0)
      {
        std::cout << "Every complex value is solution." << std::endl;
      }
      else
      {
        std::cout << "There is no solution" << std::endl;
      }
    }
    else
    {
      Sdouble x1 = - c/b;
      std::cout << "The equation is degenerated. There is one real64 solution" << x1 << std::endl;
    }
  }
  else
  {
    b = b/a;
    c = c/a;
    Sdouble d = b*b - 4.0*c;
    std::cout << "d = " << d << std::endl;
    //displayError(d, 0);

    if (d == 0.0)
    {
      Sdouble x1 = -b*0.5;
      std::cout << "Discriminant is zero. The double solution is " << x1 << std::endl;
      //displayError(x1, 3.5);
    }
    else if (d > 0.0)
    {
      Sdouble x1 = ( - b - sqrt(d))*0.5;
      Sdouble x2 = ( - b + sqrt(d))*0.5;
      std::cout << "There are two real64 solutions. x1 = " << x1 << " x2 = " << x2 << std::endl;
      //displayError(x1, 3.5);
      //displayError(x2, 3.5);
    }
    else
    {
      Sdouble x1 = - b*0.5;
      Sdouble x2 = sqrt(-d)*0.5;
      std::cout << "There are two complex solutions."
                << "z1 = " << x1 << " + i * " << x2 << ' '
                << "z2 = " << x1 << " + i * " << -x2
                << std::endl;
      //displayError(x1, 3.5);
    }
  }

  std::cout << std::endl;
}

int main() {
  secondOrder();
  return EXIT_SUCCESS;
}