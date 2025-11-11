struct BITRAQRSQ {
  BIT fw0, fw1;
  BITRAQRSQ(int n) : fw0(n), fw1(n) {}
  void add_range(int l, int r, ll w) {
    fw0.add(l, -w * l); fw0.add(r, w * r);
    fw1.add(l, w); fw1.add(r, -w);
  }
  ll pref(int r) { return fw0.sum(r) + (ll)r*fw1.sum(r); }
  ll sum(int l, int r) { return pref(r) - pref(l); }
};