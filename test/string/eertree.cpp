// https://judge.yosupo.jp/submission/356423
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/string/eertree.cpp"

// -----------------------

void solve() {
    string S;
    cin >> S;

    EerTree et;
    for (char c : S) {
        et.add(c);
    }

    // 1. 固有の回文の数 n を出力
    int n = et.count_unique();
    cout << n << "\n";

    // 2. 各頂点 v (1 <= v <= n) の親 pv と suffix link sv を出力
    // ライブラリ内のインデックス 2 から n+1 が、問題の頂点 1 から n に対応します
    for (int i = 2; i < (int)et.ch.size(); ++i) {
        cout << et.par[i] - 1 << " " << et.link[i] - 1 << "\n";
    }

    // 3. 各接頭辞 S[0..i] の最大回文接尾辞の頂点番号を出力
    for (int i = 0; i < (int)S.size(); ++i) {
        cout << et.suff[i] - 1 << (i == (int)S.size() - 1 ? "" : " ");
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout << fixed << setprecision(20);

    solve();

    return 0;
}
