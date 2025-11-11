#include <bits/stdc++.h>
using namespace std;
struct LCAAuxiliaryTree {
  int n;
  vector<int> fs, ls;
  LCA tree;
  LCAAuxiliaryTree(vector<vector<int>> G) : n(G.size()), fs(n, -1), ls(n, -1), tree(G, 0) {
    int time = 0;
    auto dfs = [&] (auto &&dfs, int v,int p){
      fs[v] = time++;
      for(int x : G[v]) if (x != p) {
        dfs(dfs, x, v);
      }
      ls[v] = time-1;
    };
    dfs(dfs, 0, -1);
  }
  int build(vector<int> vs, vector<vector<int>> &RES) {
    assert(RES.size() == n);
    auto cmp = [&] (int x, int y) { return fs[x] < fs[y]; };
    sort(vs.begin(), vs.end(), cmp);
    int k = vs.size();
    rep(i, k-1) vs.push_back(tree.lca(vs[i], vs[i+1]));
    sort(vs.begin(), vs.end(), cmp);
    vs.erase(unique(vs.begin(), vs.end()), vs.end());
    stack<int> s;
    int root = -1;
    for(int v : vs) {
      while (!s.empty() && ls[s.top()] < fs[v]) s.pop();
      if(!s.empty()) RES[s.top()].push_back(v);
      else root = v;
      s.push(v);
    }
    return root;
  }
};