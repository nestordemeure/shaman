#include <cmath>
#include <functional>
#include "../../shaman/Shaman.h"

typedef Sfloat RealType;

/** Calcule une transformation sans erreur pour l'addition, à l'aide de
    l'algorithme de FastTwoSum:

    a + b = c + err

    @param [in]  a    première opérande
    @param [in]  b    deuxième opérande
    @param [out] err  erreur
    @return      c    somme
 */
RealType fastTwoSum (RealType a, RealType b, RealType& err)
{
    if (abs(b) > abs(a))
    {
        std::swap(a,b);
    }
    RealType c = a + b;
    RealType z = c - a;
    err = b - z;
    return c;
}

/** Calcule l'intégrale d'une fonction entre deux bornes avec la méthode des
    rectangles.

    @param f   la fonction à intégrer
    @param x0  borne gauche de l'intervalle d'intégration
    @param x1  borne droite
    @param n   nombre de rectangles
 */
template<typename T>
RealType integrate(const T &f, RealType a, RealType b, unsigned int n)
{
    // Pas d'intégration
    const RealType dx = (b - a) / n;

    // Accumulateur
    RealType sum = 0.;

    // Boucle sur les rectangles d'intégration
    for (RealType x = a + 0.5 * dx; x < b; x += dx)
    {
        sum += dx * f(x);
    }

    // integer loop
    /*
    for (unsigned int i = 0 ; i < n ; i++)
    {
        RealType x = a + (i+0.5) * dx;
        RealType tmp = dx * f(x);
        sum += tmp;
    }
    */

    // compensated sum
    /*
    RealType sumErr = 0.;
    for (unsigned int i = 0 ; i<n ; ++i)
    {
        RealType x = a + (i+0.5) * dx;
        RealType tmp = dx * f(x);
        RealType err;
        sum = fastTwoSum(sum, tmp, err);
        sumErr += err;
    }
    sum += sumErr;
    */

    return sum;
}
