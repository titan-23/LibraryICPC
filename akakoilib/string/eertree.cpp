struct EerTree {
private:
  int last_idx, n; ll all_count;
  string s;
  vector<map<char, int>> child;
  // vector<array<int, 26>> child; // -1埋め
  vector<int> par, len, link, count, start, link_dep;
  vector<int> suff; // suff[i]:= s[:i+1]の最大回文接尾辞に対応する頂点番号(s[i]を含んで終わる)
  int new_node() {
    int idx = link.size();
    child.emplace_back(); link.emplace_back();
    len.emplace_back(0); par.emplace_back(0);
    count.emplace_back(0); start.emplace_back(0);
    link_dep.emplace_back(0);
    return idx;
  }
  int get_upper(int v, char c) const {
    while (v != 0) {
      if (n-len[v]-1 >= 0 && s[n-len[v]-1] == c) break;
      v = link[v];
    }
    return v;
  }
public:
  EerTree() : n(0), all_count(0) {
    int idx0 = new_node(); int idx1 = new_node();
    len[idx0] = -1; len[idx1] = 0;
    link[idx0] = 0; link[idx1] = idx0;
    link_dep[idx0] = 0; link_dep[idx1] = 0;
    par[idx0] = par[idx1] = 0;
    last_idx = idx1;
  }
  void add(char c) {
    s += c;
    int now = get_upper(last_idx, c);
    if (child[now].find(c) != child[now].end()) {
      last_idx = child[now][c];
      suff.emplace_back(last_idx); n++;
      count[last_idx]++;
      all_count += link_dep[last_idx];
      return;
    }
    int idx = new_node(); len[idx] = len[now]+2;
    child[now][c] = idx; par[idx] = now; count[idx] = 1;
    start[idx] = n+1-len[idx];
    suff.emplace_back(idx); last_idx = idx;
    if (len[idx] == 1) {
      link[idx] = 1;
    } else {
      int k = get_upper(link[now], c);
      link[idx] = child[k][c];
    }
    link_dep[idx] = link_dep[link[idx]]+1;
    all_count += link_dep[idx]; n++;
  }
  // s[i]で終わる回文の中で最長のもののidx
  int get_suff(int i) { return suff[i]; }
  // s[i]で終わる回文の中で最長のものの長さ
  int get_max_length_suffix(int i) { return len[suff[i]]; }
  // s[i]を末尾に持つ空でない回文の個数 / O(1)
  int count_suffix(int i) { return link_dep[suff[i]]; }
  // s[i]を末尾に持つ空でない回文のidx全列挙 / O(?)
  vector<int> enumerate_suffix(int i) {
    vector<int> res(count_suffix(i));
    int k = 0, idx = suff[i];
    while (idx > 1) {
      res[k] = idx; k++;
      idx = link[idx];
    }
    return res;
  }
  // 空でないすべての回文の個数 / O(1)
  ll count_all() { return all_count; }
  // 相異なる空でない回文の総数 / O(1)
  int count_unique() { return (int)child.size()-2; }
  // 回文のidxと頻度配列 / O(?)
  vector<int> get_freq() {
    vector<int> cnt((int)count.size()-2, 0);
    for (int i = (int)child.size()-1; i >= 2; --i) {
      cnt[i-2] += count[i];
      if (link[i]-2 >= 0) cnt[link[i]-2] += cnt[i-2];
    }
    return cnt;
  }
  // 回文のidxが文字列s[l,r)で登場する / O(1)
  pair<int, int> idx_to_range(int idx) { return {start[idx+2], start[idx+2]+len[idx+2]}; }
  // 回文のidxから文字列tへの変換 / O(|t|)
  string idx_to_string(int idx) { idx += 2; return s.substr(start[idx], len[idx]); }
  void print() {
    auto freq = get_freq();
    rep(i, count_unique()) {
      cout << i << " : " << idx_to_string(i) << ", " << freq[i] << endl;
    }
  }
};