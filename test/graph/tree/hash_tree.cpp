// https://judge.yosupo.jp/submission/356747
#include "./../../../akakoilib/template/template.cpp"
#include "./../../../akakoilib/other/random.cpp"
// #include "./../../../akakoilib/graph/tree/hashed_tree.cpp"

// vector<u64> hs(n, 1); vector<u64> R(n+1);
// rep(i, n+1) R[i] = randint(47, (1ull<<61)-1);
// auto dfs = [&] (auto &&dfs, int v, int p) -> int {
//   int d = 0;
//   for (int x : G[v]) if (x != p) {
//     int dx = dfs(dfs, x, v);
//     d = max(d, dx);
//     hs[v] = mul(hs[v], add(R[dx], hs[x]));
//   }
//   return d+1;
// };
// dfs(dfs, root, -1);

const ll mod = (1LL << 61) - 1;
ll add(ll a, ll b) { return (a += b) >= mod ? a-mod : a; }
ll mul(ll a, ll b) {
  __int128_t c = (__int128_t)a * b;
  return add(c>>61, c&mod);
}

void solve() {
    using u64 = unsigned long long;
    int n; cin >> n;
    vector<vector<int>> G(n);
    rep(i, n-1) {
        int p; cin >> p;
        G[i+1].push_back(p);
        G[p].push_back(i+1);
    }
    Random rnd;
    vector<u64> hs(n, 1); vector<u64> R(n+1);
    rep(i, n+1) R[i] = rnd.randint(47, (1ull<<61)-1);
    auto dfs = [&] (auto &&dfs, int v, int p) -> int {
        int d = 0;
        for (int x : G[v]) if (x != p) {
            int dx = dfs(dfs, x, v);
            d = max(d, dx);
            hs[v] = mul(hs[v], add(R[dx], hs[x]));
        }
        return d+1;
    };
    dfs(dfs, 0, -1);
    map<u64, int> S;
    int idx = 0;
    vector<int> ans(n);
    rep(i, n) {
        if (S.find(hs[i]) == S.end()) {
            ans[i] = idx;
            S[hs[i]] = idx;
            idx++;
        } else {
            ans[i] = S[hs[i]];
        }
    }
    rep(i, n) {
        cout << ans[i] << " ";
    }
    cout << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}
