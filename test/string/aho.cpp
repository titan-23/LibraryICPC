// https://judge.yosupo.jp/submission/356411
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/string/aho_corasic.cpp"

// -----------------------

void solve() {
    int N;
    cin >> N;

    vector<string> S(N);
    vector<int> V(N);
    AhoCorasick aho('a');

    // 文字列を追加し、終端の頂点番号を保存
    for (int i = 0; i < N; ++i) {
        cin >> S[i];
        V[i] = aho.add_string(S[i]);
    }

    int n = aho.a.size();
    vector<int> p(n, -1);

    // build() を呼ぶと ch が上書きされるため、その前に親頂点を記録
    for (int i = 0; i < n; ++i) {
        for (int c = 0; c < 26; ++c) {
            int v = aho.a[i].ch[c];
            if (v != -1) {
                p[v] = i;
            }
        }
    }

    // オートマトンと Failure Link の構築
    aho.build();

    // 出力
    cout << n << "\n";
    for (int i = 1; i < n; ++i) {
        // p[i] は事前記録した親、aho.a[i].fail は suffix link (sv)
        cout << p[i] << " " << aho.a[i].fail << "\n";
    }
    for (int i = 0; i < N; ++i) {
        cout << V[i] << (i == N - 1 ? "" : " ");
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
