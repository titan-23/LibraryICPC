template <typename T>
struct PotUF {
    vector<int> par, sz;
    vector<T> dw;
    PotUF(int n) {
        par.resize(n);
        iota(par.begin(), par.end(), 0);
        sz.assign(n, 1);
        dw.assign(n, 0);
    }
    int find(int x) {
        if (par[x] == x) return x;
        int root = find(par[x]);
        dw[x] = dw[x] + dw[par[x]];
        return par[x] = root;
    }
    bool merge(int x, int y, T w) {
        w = -dw[y] + (w + dw[x]);
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) {
            swap(x, y);
            w = -w;
        }
        par[y] = x;
        sz[x] += sz[y];
        dw[y] = w;
        return true;
    }
    T weight(int x) { return dw[find(x)]; }
    T diff(int x, int y) { return dw[y] - dw[x]; }
};