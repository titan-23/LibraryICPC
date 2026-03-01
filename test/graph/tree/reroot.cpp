// https://judge.yosupo.jp/submission/356730
#include "./../../../akakoilib/template/template.cpp"
// #include "./../../akakoilib/graph/tree/rerooting_dp.cpp"

template<typename E, typename T, T (*merge)(T, T),
    T (*apply_vertex)(T, int),
    T (*apply_edge)(T, E, int, int), T (*e)()>
vector<T> rerooting_dp(vector<vector<pair<int, E>>> G) {
  int n = G.size(), root = 0;
  vector<vector<T>> dp(n);
  rep(i, n) dp[i].resize(G[i].size(), e());
  vector<int> pdx(n, -1), topo;
  stack<int> s; s.emplace(root);
  pdx[root] = -2;
  while (!s.empty()) {
    int v = s.top(); s.pop();
    topo.emplace_back(v);
    rep(i, G[v].size()) {
      int x = G[v][i].first;
      if (pdx[x] != -1) { pdx[v] = i; continue; }
      s.emplace(x);
    }
  }
  vector<T> ans(n), rs(n+1, e());
  for (int idx = n-1; idx >= 0; --idx) {
    int v = topo[idx];
    T ls = e();
    rep(i, G[v].size()) if (i != pdx[v]) {
      auto [x, c] = G[v][i];
      dp[v][i] = apply_edge(ans[x], c, x, v);
      ls = merge(ls, dp[v][i]);
    }
    ans[v] = apply_vertex(ls, v);
  }
  for (int v : topo) {
    int d = G[v].size();
    rep(i, d) rs[i+1] = merge(rs[i], dp[v][d-i-1]);
    T ls = e();
    rep(i, d) {
      if (i != pdx[v]) {
        auto [x, c] = G[v][i];
        dp[x][pdx[x]] = apply_edge(apply_vertex(merge(ls, rs[d-i-1]), v), c, v, x);
      }
      ls = merge(ls, dp[v][i]);
    }
    ans[v] = apply_vertex(rs[d], v);
  }
  return ans;
}
// using T = int; // dpの型
// using E = int; // 辺の型
// T apply_vertex(T dp_x, int v) {} // 辺込みの部分木の値dp_xに頂点vを付加
// T merge(T s, T t) {} // 辺込みの部分木のdp値s,tを結合
// T apply_edge(T dp_x, E edge, int x, int v) {} // 部分木のdp値dp_xに辺edgeを付加 頂点xの親がv
// T e() {} // dp値の単位元

const int bit = 30;
const int msk = (1<<30)-1;
const int mod = 998244353;
vector<int> A;

using T = ll;
using E = ll;

T apply_vertex(T dp_x, int v) {
    ll s = dp_x>>bit, c = dp_x&msk;
    return ((s+A[v])%mod) << bit | (c+1);
}

T merge(T s, T t) {
    ll s0 = s>>bit, s1 = s&msk;
    ll t0 = t>>bit, t1 = t&msk;
    return ((s0+t0)%mod) << bit | ((s1+t1)%mod);
}

T apply_edge(T dp_x, E edge, int x, int v) {
    ll b0 = edge>>bit, c0 = edge&msk;
    ll s = dp_x>>bit, c = dp_x&msk;
    return ((b0*s+c0*c)%mod) << bit | c;
}

T e() {
    return 0;
}

void solve() {
    int n; cin >> n;
    A.resize(n);
    rep(i, n) {
        cin >> A[i];
    }
    vector<vector<pair<int, E>>> G(n);
    rep(_, n-1) {
        int u, v;
        E b, c;
        cin >> u >> v >> b >> c;
        G[u].emplace_back(v, b<<bit|c);
        G[v].emplace_back(u, b<<bit|c);
    }

    vector<T> dp = rerooting_dp<E, T, merge, apply_vertex, apply_edge, e>(G);
    rep(i, n) {
        cout << (dp[i]>>bit) << " ";
    }
    cout << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}