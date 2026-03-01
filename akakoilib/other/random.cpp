struct Random {
  mt19937_64 rng;
  Random() { rng.seed(12312); }
  double uniform() { // [0.0, 1.0)
    return uniform_real_distribution<double>(0.0, 1.0)(rng);
  }
  ll randint(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
  }
};