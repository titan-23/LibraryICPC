// どの点とそれに接続する辺を取り除いても連結であるようなグラフのこと
// 成分内の任意の2頂点間には、内素なパス(始点・終点以外で同じ頂点を通らないパス)が2本以上存在する
// 成分内に関節点を含まない
// 異なる三点(u, v, w)をとると、u, vとu, wをそれぞれ結ぶような点素なパスの組が存在する
// 辺はただ1つの成分に属するが、頂点(関節点)は複数の成分に属する
struct BiConnectedCC {
  int n;
  vector<vector<int>> G;
  vector<int> ord, low, arti;
  vector<vector<pair<int, int>>> bc; // 各成分に含まれる辺
  vector<vector<int>> tree; // BCTの隣接リスト
  vector<pair<int, int>> tmp;
  BiConnectedCC(vector<vector<int>> G) : n(G.size()), G(G), ord(n, -1), low(n, -1) {
    int k = 0;
    rep(i, n) if (ord[i] == -1) dfs(i, -1, k);
    build_bct();
  }
  void dfs(int v, int p, int &k) { // need `&`
    low[v] = (ord[v] = k++);
    bool is_arti = false;
    int cnt = 0;
    for (int x : G[v]) if (x != p) {
      if (ord[x] < ord[v]) tmp.emplace_back(minmax(v, x));
      if (ord[x] == -1) {
        cnt++;
        int sz = tmp.size();
        dfs(x, v, k);
        chmin(low[v], low[x]);
        if (low[x] >= ord[v]) {
          is_arti = true;
          bc.push_back({});
          while (tmp.size() >= sz) {
            bc.back().push_back(tmp.back());
            tmp.pop_back();
          }
        }
      } else chmin(low[v], ord[x]);
    }
    if (p == -1) is_arti = (cnt > 1);
    if (is_arti) arti.push_back(v);
  }
  // 頂点 n ~ n+bc.size()-1 : ブロックを表す頂点
  void build_bct() {
    tree.assign(n + bc.size(), vector<int>());
    vector<int> last(n, -1);
    rep(i, bc.size()) for (auto &[x, y] : bc[i]) {
      for (int v : {x, y}) if (last[v] != i) {
        last[v] = i;
        tree[v].push_back(n+i);
        tree[n+i].push_back(v);
      }
    }
  }
};