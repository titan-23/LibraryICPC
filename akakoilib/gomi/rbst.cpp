struct LazyRBST {
  struct Node;
  using NP = Node*; stack<NP> pL, pR; NP root;
  struct Node {
    NP l, r; T key, dat; F lazy; bool rev; int size;
    Node(T key, F lazy) : l(nullptr), r(nullptr), key(key), dat(key), lazy(lazy), rev(false), size(1) {}
    void update() {
      size = 1; dat = key;
      if (l) { size += l->size; dat = op(l->dat, dat); }
      if (r) { size += r->size; dat = op(dat, r->dat); }
    }
    void push(F f) {
      key = mapping(f, key); dat = mapping(f, dat);
      if (l || r) lazy = composition(f, lazy);
    }
    void propagate() {
      if (rev) { swap(l, r); rev = 0;
        if (l) l->rev ^= 1;
        if (r) r->rev ^= 1;
      }
      if (l) l->push(lazy);
      if (r) r->push(lazy);
      lazy = id();
    }
  };
  void _build(vector<T> const &a) {
    auto build = [&] (auto &&build, int l, int r) -> NP {
      int mid = (l + r) >> 1;
      NP v = new Node(a[mid], id());
      if (l != mid) v->l = build(build, l, mid);
      if (mid+1 != r) v->r = build(build, mid+1, r);
      v->update(); return v;
    };
    if (a.empty()) { this->root = nullptr; return; }
    this->root = build(build, 0, a.size());
  }
  void _update_lr(NP l, NP r) {
    l->size += r->size; l->dat = op(l->dat, r->dat);
  }
  NP _merge_node(NP l, NP r) {
    NP nr = nullptr, pr = nullptr;
    int d = -1;
    while (l && r) { // 乱数を始めに列挙しておくのも手
      int nd = trnd.randint(0, l->size+r->size-1) < l->size;
      NP v = nd ? l : r;
      v->propagate();
      if (!nr) { pr = v; }
      else if (d == 0) { nr->l = v; }
      else { nr->r = v; }
      nr = v; d = nd;
      if (d) { _update_lr(l, r); l = l->r; }
      else   { _update_lr(r, l); r = r->l; }
    }
    if (!nr) return l ? l : r;
    (d ? nr->r : nr->l) = l ? l : r;
    return pr;
  }
  pair<NP, NP> _split_node(NP v, int k) {
    while (v) { v->propagate();
      int s = v->l ? k-v->l->size : k;
      if (s <= 0) { pR.emplace(v); v = v->l; }
      else { k = s-1; pL.emplace(v);  v = v->r; }
    }
    NP l = nullptr, r = nullptr;
    while (!pL.empty()) {
      NP v = pL.top(); pL.pop();
      v->r = l; l = v; l->update();
    }
    while (!pR.empty()) {
      NP v = pR.top(); pR.pop();
      v->l = r; r = v; r->update();
    }
    return {l, r};
  }
  LazyRBST(NP v) : root(v) {}
public:
  LazyRBST() : root(nullptr) {}
  LazyRBST(vector<T> a) : root(nullptr) { _build(a); }
  void apply(int l, int r, F f) { if (l == r) return;
    auto [a, bc] = split(l); auto [b, c] = bc.split(r - l);
    if (b.root) b.root->push(f);
    root = _merge_node(_merge_node(a.root, b.root), c.root);
  }
  T get(int k) {
    NP v = root;
    while (1) { v->propagate();
      int t = v->l ? v->l->size : 0;
      if (t == k) return v->key;
      if (t < k) { k -= t + 1; v = v->r; }
      else { v = v->l; }
    }
  }
  void set(int k, T key) {
    NP v = root;
    while (1) { v->propagate(); pL.emplace(v);
      int t = v->l ? v->l->size : 0;
      if (t == k) { v->key = key; break; }
      if (t < k) { k -= t + 1; v = v->r; }
      else { v = v->l; }
    }
    while (!pL.empty()) { pL.top()->update(); pL.pop(); }
  }
};