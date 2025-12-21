// その成分内の任意の辺を1本削除しても、成分内が非連結にならない
// 成分内の任意の2頂点間には、辺素なパス(同じ辺を通らないパス)が2本以上存在し、成分内に橋を含まない
// 橋でない辺のみ含むようなグラフの連結成分に対応する(一意である)
// 二重辺連結成分に対応する頂点を考え、各橋について端点の属する成分の点を結んだようなグラフは木である
struct TwoEdgeCC {
  int n, num;
  vector<vector<int>> G, groups, tree;
  LowLink L;
  vector<int> id;
  TwoEdgeCC(vector<vector<int>> G) : n(G.size()), num(0), G(G), L(G), id(n, -1) {
    rep(i, n) if (id[i] == -1) {
      groups.push_back({}); dfs(i, num);
      num++;
    }
    tree.resize(num);
    rep(v, n) for (int x : G[v]) if (id[v] != id[x]) {
      tree[id[v]].push_back(id[x]);
    }
    for (auto &v : tree) {
      sort(v.begin(), v.end());
      v.erase(unique(v.begin(), v.end()), v.end());
    }
  }
  void dfs(int v, int now) {
    id[v] = now;
    groups.back().push_back(v);
    for (int x : G[v]) if (id[x] == -1) {
      if (L.ord[v] >= L.low[x] && L.ord[x] >= L.low[v]) dfs(x, now);
    }
  }
};