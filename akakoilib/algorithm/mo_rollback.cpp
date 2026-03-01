// RESET() データ構造を初期状態に戻す 回数O(√N)
// SNAPSHOT()  現在の状態を保存する 回数O(Q)
// ROLLBACK()  直前のSNAPSHOTの状態まで戻す 回数O(Q)
struct MoRollBack {
  int W; vector<int> L, R;
  MoRollBack(int n, int q) : W(max(1, (int)sqrt(n))) {}
  void add_query(int l, int r) { L.emplace_back(l); R.emplace_back(r); }
  void run(const auto &add, const auto &out, const auto &reset, const auto &snapshot, const auto &rollback) {
    vector<int> order(L.size());
    iota(begin(order), end(order), 0);
    sort(begin(order), end(order), [&] (int a, int b) {
      return L[a]/W==L[b]/W ? R[a]<R[b] : L[a]/W<L[b]/W;
    });
    reset();
    for (int idx : order) if (R[idx]-L[idx] < W) {
      snapshot();
      for (int i = L[idx]; i < R[idx]; i++) add(i);
      out(idx); rollback();
    }
    int nr = 0, last_block = -1;
    for (int idx : order) if (R[idx]-L[idx] >= W) {
      int block = L[idx] / W;
      if (last_block != block) {
        reset(); last_block = block; nr = (block + 1) * W;
      }
      while (nr < R[idx]) add(nr++);
      snapshot();
      for (int j = (block+1)*W-1; j >= L[idx]; j--) add(j);
      out(idx); rollback();
    }
  }
};