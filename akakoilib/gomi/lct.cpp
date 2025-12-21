template <class T, T (*op)(T, T), T (*e)(), class F, T (*mapp)(F, T), F (*composition)(F, F), F (*id)()>
struct LazyLinkCutTree {
private:
  struct Node; using NP = Node*; vector<NP> pool;
  struct Node {
    int idx, size = 1, rev = 0; T key, dat, rdat; F lazy;
    NP l = nullptr, r = nullptr, p = nullptr;
    Node(int i, T k, F l) : idx(i), key(k), dat(k), rdat(k), lazy(l) {}
    bool is_root() const { return !p || (p->l != this && p->r != this); }
  };
  void _apply_rev(NP v) { if (v) v->rev ^= 1; }
  void _apply_f(NP v, F f) { if (!v) return;
    v->key = mapp(f, v->key);
    v->dat = mapp(f, v->dat); v->rdat = mapp(f, v->rdat);
    v->lazy = composition(f, v->lazy);
  }
  void _push(NP v) { if (!v) return;
    if (v->rev) {
      swap(v->dat, v->rdat); swap(v->l, v->r);
      _apply_rev(v->l); _apply_rev(v->r);
      v->rev = 0;
    }
    _apply_f(v->l, v->lazy); _apply_f(v->r, v->lazy);
    v->lazy = id();
  }
  void _update(NP v) {
    if (!v) return;
    _push(v->l); _push(v->r);
    v->dat = v->key; v->rdat = v->key;
    v->size = 1;
    if (v->l) {
      v->dat = op(v->l->dat, v->dat);
      v->rdat = op(v->rdat, v->l->rdat);
      v->size += v->l->size;
    }
    if (v->r) {
      v->dat = op(v->dat, v->r->dat);
      v->rdat = op(v->r->rdat, v->rdat);
      v->size += v->r->size;
    }
  }
  void _rotate(NP v) {
    NP p = v->p; NP g = p->p;
    _push(p); _push(v);
    if (g) (g->l == p ? g->l : g->r) = v;
    v->p = g;
    if (p->l == v) {
      p->l = v->r;
      if (v->r) v->r->p = p;
      v->r = p;
    } else {
      p->r = v->l;
      if (v->l) v->l->p = p;
      v->l = p;
    }
    p->p = v; _update(p); _update(v);
  }
  void _splay(NP v) {
    while ((!v->is_root()) && (!v->p->is_root())) {
      _push(v->p->p); _push(v->p); _push(v);
      if ((v->p->p->l == v->p) == (v->p->l == v)) _rotate(v->p);
      else _rotate(v);
      _rotate(v);
    }
    if (!v->is_root()) _rotate(v);
    _push(v);
  }
  void _link(NP c, NP p) { _expose(c); _expose(p); c->p = p; p->r = c; _update(p); }
  void _cut(NP c) { _expose(c); c->l->p = nullptr; c->l = nullptr; _update(c); }
  NP _expose(const NP v) {
    NP pre = v;
    while (v->p) {
      _splay(v); v->r = nullptr; _update(v);
      if (!v->p) break;
      pre = v->p; _splay(v->p);
      v->p->r = v; _update(v->p);
    }
    v->r = nullptr; _update(v); return pre;
  }
public:
  LazyLinkCutTree(int n) : pool(n) { rep(i, n) pool[i] = new Node(i, e(), id()); }
  int expose(int v) { return _expose(pool[v])->idx; }
  void evert(int v) { NP n = pool[v]; _expose(n); _apply_rev(n); _push(n); }
  int lca(int u, int v, int root=-1) {
    if (root != -1) evert(root);
    expose(u); return expose(v);
  }
  void link(int c, int p) { _link(pool[c], pool[p]); }
  void cut(int c) { _cut(pool[c]); }
  int root(int v) {
    NP n = pool[v]; _expose(n); _push(n);
    while (n->l) { n = n->l; _push(n); }
    _splay(n); return n->idx;
  }
  bool same(int u, int v) { return root(u) == root(v); }
  T path_prod(int u, int v) { evert(u); expose(v); return pool[v]->dat; }
  void path_apply(int u, int v, F f) { evert(u); expose(v); _apply_f(pool[v], f); _push(pool[v]); }
  bool merge(int u, int v) {
    if (same(u, v)) return false;
    evert(u); link(u, v); return true;
  }
  void split(int u, int v) { evert(u); cut(v); }
  T get(int k) { _splay(pool[k]); return pool[k]->key; }
  void set(int k, T val) { NP v = pool[k]; _splay(v); v->key = val; _update(v); }
  int path_length(int s, int t) { evert(s); expose(t); NP v = pool[t]; return v->size; }
  int path_kth_elm(int s, int t, int k) {
    evert(s); expose(t); NP v = pool[t];
    if (v->size <= k) return -1;
    while (1) { _push(v);
      t = v->l ? v->l->size : 0;
      if (t == k) { _splay(v); return v->idx; }
      if (t > k) { v = v->l; }
      else { v = v->r; k -= t + 1; }
    }
  }
};