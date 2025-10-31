struct BIT {
    int n;
    vector<ll> d;
    BIT(int n) : n(n), d(n+1) {}
    BIT(vector<ll> a) : n(a.size()), d(n+1) {
        rep(i, n) d[i+1] = a[i];
        for (int i = 1; i < n; ++i) {
            if (i+(i&-i) <= n) d[i+(i&-i)] += d[i];
        }
    }
    void add(int i, ll x) {
        i++;
        while (i <= n) d[i] += x, i += i & -i;
    }
    ll sum(int r) {
        ll s = 0;
        while (r) s += d[r], r -= r & -r;
        return s;
    }
    ll sum(int l, int r) { return sum(r) - sum(l); }
    int bisect_left(ll w) {
        int i = 0, s = 1<<(32-__builtin_clz(n));
        while (s) {
            if (i+s <= n && d[i+s] < w) {
                w -= d[i+s];
                i += s;
            }
            s >>= 1;
        }
        return (w ? i : -1);
    }
    int bisect_right(ll w) {
        int i = 0, s = 1<<(32-__builtin_clz(n));
        while (s) {
            if (i+s <= n && d[i+s] <= w) {
                w -= d[i+s];
                i += s;
            }
            s >>= 1;
        }
        return i;
    }
};