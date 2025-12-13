template<typename T>
struct LinearCumSum {
  int n, B;
  vector<T> A;
  vector<vector<vector<T>>> S, X;
  LinearCumSum(vector<T> a) : n(a.size()), B((int)sqrt(n)), A(a), S(B+1), X(B+1) {
    for (int d = 1; d < B; ++d) {
      S[d].resize(d+1);
      X[d].resize(d+1);
      rep(k, d) {
        S[d][k].resize(n/d+2);
        X[d][k].resize(n/d+2);
      }
      rep(i, n) {
        S[d][i%d][i/d+1] = S[d][i%d][i/d] + A[i];
        X[d][i%d][i/d+1] = X[d][i%d][i/d] + A[i]*(i/d+1);
      }
    }
  }
  // a_l * 1 + a_{l+d} * 2 + ... + a_{l+(k-1)*d} * k
  T sum(int l, int d, int k) {
    assert(l+d*(k-1) < n);
    T s = 0;
    if (d >= B) { // naive
      int idx = 1;
      for (int i = l; i <= l+d*(k-1); i += d) {
        s += A[i] * idx;
        idx++;
      }
    } else {
      int end = l+d*(k-1);
      s = X[d][l%d][end/d+1] - X[d][l%d][l/d];
      s -= (S[d][l%d][end/d+1] - S[d][l%d][l/d]) * (l/d);
    }
    return s;
  }
};