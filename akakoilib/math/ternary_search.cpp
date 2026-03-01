double golden_search(double L, double R) { // 極小値
  const double C = 2-(1+sqrt(5))/2;
  double m1 = L + C * (R - L);
  double m2 = R - C * (R - L);
  F f1 = f(m1), f2 = f(m2);
  rep(_, 100) {
    if (f1 < f2) {
      R = m2; m2 = m1; f2 = f1;
      m1 = L + C*(R-L); f1 = f(m1);
    } else {
      L = m1; m1 = m2; f1 = f2;
      m2 = R - C*(R-L); f2 = f(m2);
    }
  }
  return (L+R)/2;
}