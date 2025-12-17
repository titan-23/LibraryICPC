namespace persistent_lazy_tree {
struct Node {
  Node const *l, *r;
  U size; F lz; T v; int dep; bool rev;
};
using NP = Node const*;
const int BSIZE = 1<<20;
vector<Node*> blocks;
Node *ptr = nullptr, *lim = nullptr;
int idx = 0;
void expand() {
  if (idx == blocks.size()) blocks.push_back(new Node[BSIZE]);
  ptr = blocks[idx];
  lim = ptr + BSIZE;
  idx++;
}
NP newnode(NP l, NP r, F lz, T v, U size, int dep, bool rev) {
  if (ptr == lim) expand();
  *ptr = Node{l, r, size, lz, v, dep, rev};
  return ptr++;
}
void reset() {
  idx = 0; ptr = nullptr; lim = nullptr;
}
U size(NP x) { return x ? x->size : 0; }
int dep(NP x) { return x ? x->dep : 0; }
T v(NP x) { return x ? x->v : e(); }
NP newpar(NP l, NP r) {
  return newnode(l, r, id(), op(v(l), v(r)), size(l) + size(r), max(dep(l), dep(r)) + 1, false);
}
NP add(NP x, F lz) {
  if (!x) return nullptr;
  return newnode(x->l, x->r, composition(lz, x->lz), mapping(lz, x->v), x->size, x->dep, x->rev);
}
NP toggle(NP x) {
  if (!x) return nullptr;
  return newnode(x->r, x->l, x->lz, x->v, x->size, x->dep, !x->rev);
}
void push_rev(NP t, NP &l, NP &r) {
  if (t->rev) { l = toggle(t->l); r = toggle(t->r); }
  else { l = t->l; r = t->r; }
}
pair<NP, NP> sub(NP a, NP b, NP c) {
  if (abs(max(dep(a), dep(b))+1-dep(c)) <= 1) return {newpar(a, b), c};
  if (abs(max(dep(b), dep(c))+1-dep(a)) <= 1) return {a, newpar(b, c)};
  NP bl, br; push_rev(b, bl, br);
  return {newpar(a, add(bl, b->lz)), newpar(add(br, b->lz), c)};
}
pair<NP, NP> merge2(NP a, NP b, F alz, F blz) {
  if (dep(a) < dep(b)) {
    blz = composition(blz, b->lz);
    NP bl, br; push_rev(b, bl, br);
    pair<NP, NP> x = merge2(a, bl, alz, blz);
    return sub(x.first, x.second, add(br, blz));
  } else if (dep(a) > dep(b)) {
    alz = composition(alz, a->lz);
    NP al, ar; push_rev(a, al, ar);
    pair<NP, NP> x = merge2(ar, b, alz, blz);
    return sub(add(al, alz), x.first, x.second);
  } else { return {add(a, alz), add(b, blz)}; }
}
NP merge(NP a, NP b) {
  if (!a) return b;
  if (!b) return a;
  pair<NP, NP> x = merge2(a, b, id(), id());
  return newpar(x.first, x.second);
}
pair<NP, NP> split(NP x, U s, F lz) {
  if (size(x) == s) return {add(x, lz), nullptr};
  if (s == 0) return {nullptr, add(x, lz)};
  lz = composition(lz, x->lz);
  NP xl, xr; push_rev(x, xl, xr);
  if (s <= size(xl)) {
    auto [l, r] = split(xl, s, lz);
    return {l, merge(r, add(xr, lz))};
  } else {
    auto [l, r] = split(xr, s-size(xl), lz);
    return {merge(add(xl, lz), l), r};
  }
}
vector<T> tovector(NP root) {
  vector<T> res;
  if (!root) return res;
  stack<tuple<NP, F, bool>> st;
  st.emplace(root, id(), false);
  while (!st.empty()) {
    auto [t, lz, flip] = st.top(); st.pop();
    if (t == nullptr) continue;
    if (t->size == 1) {
      res.push_back(mapping(lz, t->v));
    } else {
      F f = composition(lz, t->lz);
      bool nf = flip ^ t->rev;
      st.emplace(nf ? t->l : t->r, f, nf);
      st.emplace(nf ? t->r : t->l, f, nf);
    }
  }
  return res;
}
struct Tree {
  NP root;
  Tree() : root(nullptr) {}
  Tree(NP r) : root(r) {}
  Tree(vector<T> &v) {
    if (v.empty()) { root = nullptr; return; }
    auto dfs = [&](auto &&dfs, int l, int r) -> NP {
      if (l >= r) return nullptr;
      if (r - l == 1) return newnode(nullptr, nullptr, id(), v[l], 1, 0, false);
      int m = (l + r) / 2;
      NP L = dfs(dfs, l, m);
      NP R = dfs(dfs, m, r);
      return newpar(L, R);
    };
    root = dfs(dfs, 0, v.size());
  }
  U size() { return persistent_lazy_tree::size(root); }
  T get(U k) {
    NP t = root; F lz = id(); bool r = false;
    while (1) {
      lz = composition(lz, t->lz); r ^= t->rev;
      if (t->size == 1) return mapping(lz, t->v);
      NP lch = r ? t->r : t->l;
      U ls = lch ? lch->size : 0;
      if (k < ls) { t = lch; }
      else { k -= ls; t = r ? t->l : t->r; }
    }
  }
  Tree set(U k, T val) {
    auto push = [&] (NP n, bool r, F f) -> NP {
      if (!n) return nullptr;
      return newnode(n->l, n->r, composition(f, n->lz), mapping(f, n->v), n->size, n->dep, n->rev ^ r);
    };
    auto dfs = [&](auto self, NP t, U k) -> NP {
      if (t->size == 1) return newnode(nullptr, nullptr, id(), val, 1, 0, false);
      NP l = t->l, r = t->r;
      if (t->rev) swap(l, r);
      l = push(l, t->rev, t->lz);
      r = push(r, t->rev, t->lz);
      U sz = l ? l->size : 0;
      if (k < sz) l = self(self, l, k);
      else r = self(self, r, k - sz);
      return newpar(l, r);
    };
    return Tree(dfs(dfs, root, k));
  }
  Tree insert(U k, T v) {
    auto [l, r] = split(root, k, id());
    return Tree(merge(merge(l, newnode(nullptr, nullptr, id(), v, 1, 0, false)), r));
  }
  Tree erase(U l, U r) {
    auto [abc, d] = split(root, r, id());
    auto [a, bc] = split(abc, l, id());
    return Tree(merge(a, d));
  }
  // range query
  // auto [abc, d] = split(root, r, id());
  // auto [a, b] = split(abc, l, id());
  // T prod(U l, U r) return v(b);
  // Tree apply(U l, U r, F f) return Tree(merge(merge(a, add(b, f)), d));
  // Tree reverse(U l, U r) return Tree(merge(merge(a, toggle(b)), d));
  vector<T> to_vector() { return tovector(root); }
  void rebuild() {
    if (size() == 0) { reset(); root = nullptr; return; }
    vector<T> data = to_vector();
    reset();
    *this = Tree(data);
  }
};
void reserve(int n) {
  while (blocks.size()*BSIZE < n) expand();
}
pair<Tree, Tree> split(const Tree& t, U k) {
  auto [l, r] = split(t.root, k, id());
  return { Tree(l), Tree(r) };
}
Tree merge(const Tree& a, const Tree& b) { return Tree(merge(a.root, b.root)); }
}