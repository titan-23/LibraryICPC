struct LCAAuxiliaryTree {
  int n; vector<int> in, out; LCA tree;
  LCAAuxiliaryTree(vector<vector<int>> G) : n(G.size()), in(n, -1), out(n, -1), tree(G, 0) {
    int t = 0;
    auto dfs = [&] (auto &&dfs, int v,int p) {
      in[v] = t++;
      for (int x : G[v]) if (x != p) dfs(dfs, x, v);
      out[v] = t-1;
    };
    dfs(dfs, 0, -1);
  }
  int build(vector<int> vs, vector<vector<int>> &RES) {
    assert(RES.size() == n);
    auto cmp = [&] (int x, int y) { return in[x] < in[y]; };
    sort(vs.begin(), vs.end(), cmp);
    int k = vs.size(), root = -1;
    rep(i, k-1) vs.push_back(tree.lca(vs[i], vs[i+1]));
    sort(vs.begin(), vs.end(), cmp);
    vs.erase(unique(vs.begin(), vs.end()), vs.end());
    stack<int> s;
    for(int v : vs) {
      while (!s.empty() && out[s.top()] < in[v]) s.pop();
      if (!s.empty()) RES[s.top()].push_back(v);
      else root = v;
      s.push(v);
    }
    return root;
  }
};