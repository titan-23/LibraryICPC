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
T fibonacci_search(T min_val, T max_val) {  // 極小値
  auto get_val = [&](T x) {
    return x >= max_val ? 4e18 : f(x);
  };
  vector<T> fib = {1, 1, 2, 3};
  while (fib.back() <= max_val - min_val) fib.push_back(fib.back() + fib[fib.size()-2]);
  T low = min_val;
  int k = fib.size() - 1;
  T m1 = low + fib[k-2], m2 = low + fib[k-1];
  auto v1 = get_val(m1), v2 = get_val(m2);
  while (k > 2) {
    k--;
    if (v1 < v2) {
      m2 = m1; v2 = v1; m1 = low+fib[k-2];
      v1 = get_val(m1);
    } else {
      low = m1; m1 = m2; v1 = v2; m2 = low+fib[k-1];
      v2 = get_val(m2);
    }
  }
  return v1 < v2 ? m1 : m2;
}