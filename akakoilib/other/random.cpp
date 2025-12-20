struct Random {
  mt19937_64 rng;
  Random() { rng.seed(12312); }
  ll randint(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
  }
};