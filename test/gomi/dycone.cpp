// https://atcoder.jp/contests/abc334/submissions/73753480
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)

const ll dy[] = {-1, 0, 0, 1};
const ll dx[] = {0, -1, 1, 0};
const int mod = 998244353;
// --------

#include "./../../akakoilib/math/modint.cpp"
#include "./../../akakoilib/gomi/dycone.cpp"


void solve() {
  int h, w;
  cin >> h >> w;
  vector<string> S(h);
  rep(i, h) cin >> S[i];

  vector<vector<int>> A(h, vector<int>(w, 0));
  rep(i, h) rep(j, w) {
    if (S[i][j] == '#') {
      A[i][j] = 1;
    }
  }

  Dycone<ll> dc(h*w, 11*h*w);

  vector<int> IDX;

  vector<pair<int, int>> edge;
  rep(i, h) rep(j, w) {
    if (!A[i][j]) continue;
    rep(d, 2) {
      int ni = i + dy[d];
      int nj = j + dx[d];
      if (0 <= ni && ni < h && 0 <= nj && nj < w) {
        if (A[ni][nj]) {
          edge.emplace_back(i*w+j, ni*w+nj);
        }
      }
    }
  }
  sort(edge.begin(), edge.end());
  edge.erase(unique(edge.begin(), edge.end()), edge.end());
  for (auto &[u, v]: edge) {
    dc.add_edge(u, v);
  }

  mint<mod> green = 0;
  rep(i, h) rep(j, w) {
    if (!A[i][j]) continue;
    green += 1;

    rep(d, 4) {
      int ni = i + dy[d], nj = j + dx[d];
      if (0 <= ni && ni < h && 0 <= nj && nj < w) {
        if (A[ni][nj]) {
          dc.delete_edge(i*w+j, ni*w+nj);
        }
      }
    }

    int t = dc.get_component_count();
    IDX.push_back(t);

    rep(d, 4) {
      int ni = i + dy[d], nj = j + dx[d];
      if (0 <= ni && ni < h && 0 <= nj && nj < w) {
        if (A[ni][nj]) {
          dc.add_edge(i*w+j, ni*w+nj);
        }
      }
    }
  }

  mint<mod> ans = 0;
  mint<mod> red = mint<mod>(h*w)- green;

  auto res = dc.run();
  for (int idx : IDX) {
    ans += mint<mod>(res[idx])-mint<mod>(red)-mint<mod>(1);
  }

  ans = ans / green;
  cout << ans.x << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
  return 0;
}