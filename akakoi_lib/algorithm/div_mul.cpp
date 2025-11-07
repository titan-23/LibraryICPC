vector<int> primes;
// 倍数高速ゼータ変換（倍数についての和をまとめる）
vector<ll> MultipleZeta(vector<ll> v) {
  int n = v.size() - 1;
  for (int p : primes) for (int k = n / p; k > 0; k--) v[k] += v[k * p];
  return v;
}
// 約数高速ゼータ変換
vector<ll> DvisorZeta(vector<ll> v) {
  int n = v.size() - 1;
  for (int p : primes) for (int k = 1; k * p <= n; k++) v[k * p] += v[k];
  return v;
}
// 倍数高速メビウス変換
vector<ll> MultipleMobius(vector<ll> v) {
  int n = v.size() - 1;
  for (int p : primes) for (int k = 1; k * p <= n; k++) v[k] -= v[k * p];
  return v;
}
// 約数高速メビウス変換
vector<ll> DivisorMobius(vector<ll> v) {
  int n = v.size() - 1;
  for (int p : primes) for (int k = n / p; k > 0; k--) v[k * p] -= v[k];
  return v;
}