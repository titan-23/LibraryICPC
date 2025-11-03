template <typename T, T (*op)(T, T), T (*e)()>
struct Segtree {
    int n, s;
    vector<T> d;
    Segtree(vector<T> a) : n(a.size()) {
        s = 1;
        while (s < n) s <<= 1;
        d.resize(2*s, e());
        rep(i, n) d[i+s] = a[i];
        for (int i = s-1; i > 0; --i) d[i] = op(d[i<<1], d[i<<1|1]);
    }
    void set(int i, T x) {
        i += s;
        d[i] = x;
        while (i) {
            i >>= 1;
            d[i] = op(d[i<<1], d[i<<1|1]);
        }
    }
    T prod(int l, int r) {
        T lv = e(), rv = e();
        l += s; r += s;
        while (l < r) {
            if (l & 1) lv = op(lv, d[l++]);
            if (r & 1) rv = op(d[--r], rv);
            l >>= 1;
            r >>= 1;
        }
        return op(lv, rv);
    }
    T all_prod() { return d[1]; }
    template <typename F>
    int max_right(int l, F f) {
        assert(f(e()));
        T v = e();
        l += s;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(v, d[l]))) {
                while (l < s) {
                    l = (2 * l);
                    if (f(op(v, d[l]))) {
                        v = op(v, d[l]);
                        l++;
                    }
                }
                return l - s;
            }
            v = op(v, d[l]);
            l++;
        } while ((l & -l) != l);
        return s;
    }
    template <typename F>
    int min_left(int r, F f) {
        assert(f(e()));
        T v = e();
        r += s;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], v))) {
                while (r < s) {
                    r = r << 1 | 1;
                    if (f(op(d[r], v))) {
                        v = op(d[r], v);
                        r--;
                    }
                }
                return r + 1 - s;
            }
            v = op(d[r], v);
        } while ((r & -r) != r);
        return 0;
    }
};