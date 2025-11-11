struct Random {
  mt19937_64 rng;
  Random() {rng.seed(chrono::steady_clock::now().time_since_epoch().count()); }
  int randint(int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    return dist(rng);
  }
};