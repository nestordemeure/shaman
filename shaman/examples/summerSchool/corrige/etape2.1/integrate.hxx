typedef float RealType;

/** Calcule l'intégrale d'une fonction entre deux bornes avec la méthode des
    rectangles.

    @param f   la fonction à intégrer
    @param x0  borne gauche de l'intervalle d'intégration
    @param x1  borne droite
    @param n   nombre de rectangles
 */
template <typename T>
RealType integrate (const T& f, RealType a, RealType b, unsigned int n) {
  // Pas d'intégration
  const RealType dx = (b - a) / n;

  // Accumulateur
  RealType sum = 0.;

  // Boucle sur les rectangles d'intégration
  for (RealType x = a + 0.5 * dx ;
       x < b ;
       x += dx) {
    sum += dx * f(x);
  }

  return sum;
}
