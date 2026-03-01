// https://judge.yosupo.jp/submission/356426
#include "./../../akakoilib/template/template.cpp"
#include "./../../akakoilib/string/manacher.cpp"

// -----------------------

void solve() {
    string S;
    cin >> S;

    // 文字の間および両端にダミー文字 '$' を挿入し、長さ 2N+1 の文字列を構築
    string T = "";
    for (char c : S) {
        T += '$';
        T += c;
    }
    T += '$';

    // Manacherを実行
    vector<int> r = Manacher(T);

    // T のインデックス 1 から 2N-1 が、問題で要求される 2N-1 個の中心位置に完全に対応する
    // ダミー文字を含めた半径 r[i] から 1 を引くことで、元の文字列 S における回文の長さになる
    int limit = (int)T.size() - 1;
    for (int i = 1; i < limit; ++i) {
        cout << r[i] - 1 << (i == limit - 1 ? "" : " ");
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
