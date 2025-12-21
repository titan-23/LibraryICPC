struct ModComb {
  vector<mint> fact, factinv, inv;
  ModComb(int n) : fact(n+2), factinv(n+2), inv(n+2) {
    const int mod = mint::mod();
    fact[0] = fact[1] = mint(1);
    factinv[0] = factinv[1] = mint(1);
    inv[0] = mint(0); inv[1] = mint(1);
    for (int i = 2; i <= n; ++i) {
      fact[i] = fact[i-1] * i;
      inv[i] = -inv[mod%i] * (mod/i);
      factinv[i] = factinv[i-1] * inv[i];
    }
  }
  mint nPr(int n, int r) const {
    if (r < 0 || n < r) return mint(0);
    return fact[n] * factinv[n-r];
  }
  mint nCr(int n, int r) const {
    if (r < 0 || n < r) return mint(0);
    return fact[n] * factinv[r] * factinv[n-r];
  }
  mint nHr(int n, int r) const { return nCr(n+r-1, r); }
};