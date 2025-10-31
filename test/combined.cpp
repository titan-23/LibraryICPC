// --- Start of include: ./../../akakoi_lib/template/template.cpp ---
// #pragma GCC target("avx2")
// #pragma GCC optimize("O3,unroll-loops")
// (memo) #pragma pack(push, 1) #pragma pack(pop)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i,n) for(ll i=0;i<(ll)(n);++i)
bool chmin(auto& a, auto b) {return a>b?a=b,1:0;}
bool chmax(auto& a, auto b) {return a<b?a=b,1:0;}
// int main() {
//     ios::sync_with_stdio(false);
//     cin.tie(0);
// }
// --- End of include: ./../../akakoi_lib/template/template.cpp ---
// --- Start of include: ./../../akakoi_lib/algorithm/mo.cpp ---
struct Mo {
    int n;
    vector<pair<int, int>> lr;
    Mo(int n) : n(n) {}
    void add_query(int l, int r) { lr.emplace_back(l, r); }
    template<typename add_l, typename add_r, typename erase_l, typename erase_r, typename output>
    void run(const add_l& al, const add_r& ar, const erase_l& el, const erase_r& er, const output& out) {
        int q = lr.size();
        int block_size = max(1, n / max<int>(1, sqrt(q)));
        vector<int> ord(q);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            int ba = lr[a].first / block_size, bb = lr[b].first / block_size;
            if (ba != bb) return ba < bb;
            return (ba & 1) ? lr[a].second > lr[b].second : lr[a].second < lr[b].second;
        });
        int l = 0, r = 0;
        for(auto i : ord) {
            while(l > lr[i].first) al(--l);   // 左端を拡張
            while(r < lr[i].second) ar(r++);  // 右端を拡張
            while(l < lr[i].first) el(l++);   // 左端を縮小
            while(r > lr[i].second) er(--r);  // 右端を縮小
            out(i);
        }
    }
};
// --- End of include: ./../../akakoi_lib/algorithm/mo.cpp ---

// --- Start of include: ./../../akakoi_lib/ds/fenwick_tree.cpp ---
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
// --- End of include: ./../../akakoi_lib/ds/fenwick_tree.cpp ---

void solve() {
    int n, q; cin >> n >> q;
    vector<int> A(n);
    rep(i, n) cin >> A[i];
    Mo mo(n);
    rep(qdx, q) {
        int l, r; cin >> l >> r;
        mo.add_query(l, r);
    }

    vector<int> S = A;
    sort(A.begin(), A.end());
    A.erase(unique(A.begin(), A.end()), A.end());
    auto getA = [&] (int a) -> int {
        return lower_bound(A.begin(), A.end(), a) - A.begin();
    };
    vector<int> B(n);
    rep(i, n) {
        B[i] = getA(S[i]);
    }

    vector<ll> ans(q);
    BIT fw(n);
    ll res = 0;

    auto add_left = [&] (int i) -> void {
        res += fw.sum(0, B[i]);
        fw.add(B[i], 1);
    };
    auto add_right = [&] (int i) -> void {
        res += fw.sum(B[i]+1, n);
        fw.add(B[i], 1);
    };
    auto del_left = [&] (int i) -> void {
        res -= fw.sum(0, B[i]);
        fw.add(B[i], -1);
    };
    auto del_right = [&] (int i) -> void {
        res -= fw.sum(B[i]+1, n);
        fw.add(B[i], -1);
    };
    auto out = [&] (int i) -> void { ans[i] = res; };
    mo.run(add_left, add_right, del_left, del_right, out);
    rep(i, q) {
        cout << ans[i] << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve();
}
