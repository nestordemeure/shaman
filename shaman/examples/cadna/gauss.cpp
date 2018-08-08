//
// Created by chevalier on 08/08/18.
//

//---------------------------------------------------------------------------------------
// The gaussian method

#include <ostream>
#include <array>

#include <shaman.h>

using Sstd::abs;

/*
 * The gaussian method
 *
 * During the reduction of the third column, A(3,3) is equal to 4864. But the exact value of A(3,3) is zero. The standard foating point arithmetic cannot detect that A(3,3) is not significant.
 * This value is chosen as pivot. That leads to erroneous results.
 * CADNA detects the no significant value of A(3,3). This value is eliminated as pivot. That leads to satisfying results.
 *
 * http://www-pequan.lip6.fr/cadna/Examples_Dir/ex6.php
 */
void gauss()
{
  std::cout << "the gaussian method" << '\n'
            << "During the reduction of the third column, A(3,3) is equal to 4864. But the exact value of A(3,3) is zero. The standard foating point arithmetic cannot detect that A(3,3) is not significant. This value is chosen as pivot. That leads to erroneous results." << std::endl;

  std::array<std::array<Sfloat, 5>, 4> a;
  std::array<Sfloat, 4> xsol;
  int idim = a.size();
  int idim1 = a[0].size();

  xsol[0] = 1.f;
  xsol[1] = 1.f;
  xsol[2] = 1.e-8f;
  xsol[3] = 1.f;
  a[0][0] = 21.0f;
  a[0][1] = 130.0f;
  a[0][2] = 0.0f;
  a[0][3] = 2.1f;
  a[0][4] = 153.1f;
  a[1][0] = 13.0f;
  a[1][1] = 80.0f;
  a[1][2] = 4.74e+8f;
  a[1][3] = 752.0f;
  a[1][4] = 849.74f;
  a[2][0] = 0.0f;
  a[2][1] = -0.4f;
  a[2][2] = 3.9816e+8f;
  a[2][3] = 4.2f;
  a[2][4] = 7.7816f;
  a[3][0] = 0.0f;
  a[3][1] = 0.0f;
  a[3][2] = 1.7f;
  a[3][3] = 9.0e-9f;
  a[3][4] = 2.6e-8f;

  for (int i = 1; i<idim; i++)
  {
    Sfloat aux;
    int ll = 0;

    Sfloat pmax = 0.0f;
    for (int j = i; j <= idim; j++)
    {
      if (abs(a[j-1][i-1]) > pmax)
      {
        pmax = abs(a[j-1][i-1]);
        ll = j;
      }
    }

    if (ll != i)
    {
      for (int j = i; j <= idim1; j++)
      {
        aux = a[i-1][j-1];
        a[i-1][j-1] = a[ll-1][j-1];
        a[ll-1][j-1] = aux;
      }
    }

    aux = a[i-1][i-1];
    for (int j = i+1; j <= idim1; j++)
    {
      a[i-1][j-1] = a[i-1][j-1] / aux;
    }

    for (int k = i+1; k<=idim; k++)
    {
      aux = a[k-1][i-1];
      for (int j = i+1; j<=idim1; j++)
      {
        a[k-1][j-1] = a[k-1][j-1] - aux*a[i-1][j-1];
      }
    }
  }

  a[idim-1][idim1-1] = a[idim-1][idim1-1] / a[idim-1][idim-1];

  for (int i = idim-1; i >= 1; i--)
  {
    for (int j = i+1; j <= idim; j++)
    {
      a[i-1][idim1-1] = a[i-1][idim1-1] - a[i-1][j-1] * a[j-1][idim1-1];
    }
  }

  for (int i = 1; i<= idim; i++)
  {
    std::cout << "x_sol(" << i << ") = " << a[i-1][idim1-1] << " (true value : " << (double) xsol[i-1] << ')' << std::endl;
    //displayError(a[i-1][idim1-1], (double) xsol[i-1]);
  }

  std::cout << std::endl;
}

int main()
{
  gauss();
  return EXIT_SUCCESS;
}