// 高速ゼータ変換（下位集合の和）
vector<ll> SubsetZeta(vector<ll> v) {
    int n = __lg(v.size());
    rep(i, n) rep(j, 1 << n) if (j >> i & 1) v[j] += v[j ^ (1 << i)];
    return v;
}
// 高速ゼータ変換（上位集合の和）
template <typename Monoid>
vector<ll> SupersetZeta(vector<ll> v) {
    int n = __lg(v.size());
    rep(i, n) rep(j, 1 << n) if (~j >> i & 1) v[j] += v[j ^ (1 << i)];
    return v;
}
// 高速メビウス変換（下位集合）
vector<ll> SubsetMobius(vector<ll> v) {
    int n = __lg(v.size());
    for (int i = 0; i < n; i++) rep(j, 1 << n) if (j >> i & 1) v[j] -= v[j ^ (1 << i)];
    return v;
}
// 高速メビウス変換（上位集合）
vector<ll> SupersetMobius(vector<ll> v) {
    int n = __lg(v.size());
    rep(i, n) rep(j, 1 << n) if (~j >> i & 1) v[j] -= v[j ^ (1 << i)];
    return v;
}