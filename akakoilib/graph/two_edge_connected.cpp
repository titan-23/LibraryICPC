// 二重辺連結成分分解
// その成分内の任意の辺を1本削除しても、成分内が非連結にならないような部分グラフ
// 成分内の任意の2頂点間には、辺素なパス(同じ辺を通らないパス)が2本以上存在する
// 成分内に橋を含まない
// 橋でない辺のみ含むようなグラフの連結成分に対応する(一意である)
// 二重辺連結成分に対応する頂点を考え、各橋について端点の属する成分の点を結んだようなグラフは木である
struct TwoEdgeCC {
  int n;
  int num;
  vector<vector<int>> G;
  vector<int> ord, low, id;
  vector<vector<int>> groups;
  vector<vector<int>> tree;
  vector<int> st;
  TwoEdgeCC(vector<vector<int>> G) : n(G.size()), num(0), G(G), ord(n, -1), low(n), id(n, -1) {
    int k = 0;
    rep(i, n) if (ord[i] == -1) {
      dfs(i, -1, k);
    }
    tree.resize(num);
    rep(v, n) for (int x : G[v]) if (id[v] != id[x]) {
      tree[id[v]].push_back(id[x]);
    }
    for (auto &v : tree) { // need `&`
      sort(v.begin(), v.end());
      v.erase(unique(v.begin(), v.end()), v.end());
    }
  }
  void dfs(int v, int p, int &k) { // need `&`
    ord[v] = low[v] = k++;
    st.push_back(v);
    bool second = false;
    for (int x : G[v]) {
      if (x == p && !second) {
        second = true;
        continue;
      }
      if (ord[x] != -1) {
        low[v] = min(low[v], ord[x]);
      } else {
        dfs(x, v, k);
        low[v] = min(low[v], low[x]);
      }
    }
    if (low[v] == ord[v]) {
      groups.push_back({});
      while (1) {
        int x = st.back();
        st.pop_back();
        id[x] = num;
        groups.back().push_back(x);
        if (x == v) break;
      }
      num++;
    }
  }
};