#include <vector>
#include <cassert>
using namespace std;

template <class T, T (*op)(T, T), T (*e)(),
        class F, T (*mapping)(F, T), F (*composition)(F, F), F (*id)()>
struct LazySegtree {
private:
    int n, log, s;
    vector<T> d;
    vector<F> lazy;

    void update(int k) {  d[k] = op(d[k<<1], d[k<<1|1]); }

    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        if (k < s) lazy[k] = composition(f, lazy[k]);
    }

    void propagate(int k) {
        all_apply(k<<1, lazy[k]);
        all_apply(k<<1|1, lazy[k]);
        lazy[k] = id();
    }

    void upper_push(int l, int r) {
        for (int i = log; i > 0; --i) {
            if ((l>>i<<i) != l) propagate(l>>i);
            if ((r>>i<<i) != r) propagate((r-1)>>i);
        }
    }

public:
    LazySegtree(int n) : n(n) {
        s = 1;
        log = 0;
        while (s < n) s <<= 1, log++;
        d.resize(s<<1, e());
        lazy.resize(s, id());
    }

    LazySegtree(const vector<T> a) : n(a.size()) {
        s = 1;
        log = 0;
        while (s < n) s <<= 1, log++;
        d.resize(2*s, e());
        lazy.resize(s, id());
        rep(i, n) d[i+s] = a[i];
        for (int i = s-1; i > 0; --i) update(i);
    }

    void apply(int k, F f) {
        k += s;
        for (int i = log; i > 0; --i) propagate(k>>i);
        d[k] = mapping(f, d[k]);
        for (int i = 1; i <= log; ++i) update(k>>i);
    }

    void apply(int l, int r, F f) {
        if (l == r) return;
        l += s;
        r += s;
        upper_push(l, r);
        int pl = l, pr = r;
        while (l < r) {
            if (l & 1) all_apply(l++, f);
            if (r & 1) all_apply(--r, f);
            l >>= 1;
            r >>= 1;
        };
        l = pl;
        r = pr;
        for (int i = 1; i <= log; ++i) {
            if ((l>>i<<i) != l) update(l>>i);
            if ((r>>i<<i) != r) update((r-1)>>i);
        }
    }

    void all_apply(F f) {
        all_apply(1, f);
    }

    T prod(int l, int r) {
        if (l == r) return e();
        l += s;
        r += s;
        upper_push(l, r);
        T lv = e(), rv = e();
        while (l < r) {
            if (l & 1) lv = op(lv, d[l++]);
            if (r & 1) rv = op(d[--r], rv);
            l >>= 1;
            r >>= 1;
        }
        return op(lv, rv);
    }

    T all_prod() {
        return d[1];
    }

    T get(int k) {
        k += s;
        for (int i = log; i > 0; --i) propagate(k>>i);
        return d[k];
    }

    void set(int k, T v) {
        k += s;
        for (int i = log; i > 0; --i) propagate(k>>i);
        d[k] = v;
        for (int i = 1; i <= log; ++i) update(k>>i);
    }

    template<typename F>
    int max_right(int l, F f) {
        assert(f(e()));
        if (l == s) return n;
        l += s;
        for (int i = log; i > 0; --i) propagate(l>>i);
        T v = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(v, d[l]))) {
                while (l < s) {
                    propagate(l);
                    l <<= 1;
                    if (f(op(v, d[l]))) {
                        v = op(v, d[l]);
                        l |= 1;
                    }
                }
                return l - s;
            }
            v = op(v, d[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    template<typename F>
    int min_left(int r, F f) {
        assert(f(e()));
        if (r == 0) return 0;
        r += s;
        for (int i = log; i > 0; --i) propagate((r - 1) >> i);
        T v = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], v))) {
                while (r < s) {
                    propagate(r);
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

    vector<T> tovector() {
        for (int i = 1; i < s; ++i) propagate(i);
        return vector<T>(d.begin()+s, d.begin()+s+n);
    }
};
