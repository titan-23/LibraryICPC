// パターン検索: 根(0)からtoを辿る 途中で-1になれば存在しない
// 異なる部分文字列の総数: Σ(a[i].len - a[a[i].link].len) (1 <= i < a.size())
// 各状態の出現頻度: 初期状態の各ノードの出現を1とし、lenの降順にSuffix Linkを辿って親ノードに足し合わせる
struct SuffixAutomaton {
  struct Node {
    // len: この状態が受理する最長部分文字列の長さ
    // link: Suffix Link (自身の最長接尾辞で、別の状態に属するノードへのリンク)
    int len, link, to[26];
    Node() : len(0), link(-1) { fill(to, to+26, -1); }
  };
  vector<Node> a;
  int last;
  SuffixAutomaton() : last(0) { a.emplace_back(); }
  void add(char c) {
    int now = a.size();
    a.emplace_back();
    a[now].len = a[last].len + 1;
    int p = last, idx = c - 'a';
    while (p != -1 && a[p].to[idx] == -1) {
      a[p].to[idx] = now;
      p = a[p].link;
    }
    if (p == -1) a[now].link = 0;
    else {
      int q = a[p].to[idx];
      if (a[p].len + 1 == a[q].len) a[now].link = q;
      else {
        int clone = a.size();
        a.push_back(a[q]);
        a[clone].len = a[p].len + 1;
        while (p != -1 && a[p].to[idx] == q) {
          a[p].to[idx] = clone;
          p = a[p].link;
        }
        a[q].link = a[now].link = clone;
      }
    }
    last = now;
  }
};