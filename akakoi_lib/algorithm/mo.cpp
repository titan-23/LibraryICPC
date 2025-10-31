struct Mo {
    int n;
    vector<pair<int, int>> lr;
    Mo(int n) : n(n) {}
    void add_query(int l, int r) { lr.emplace_back(l, r); }
    template<typename add_l, typename add_r, typename erase_l, typename erase_r, typename output>
    void run(const add_l& al, const add_r& ar, const erase_l& el, const erase_r& er, const output& out) {
        int q = lr.size();
        int block_size = max(1, n / max<int>(1, sqrt(q)));
        vector<int> ord(q);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            int ba = lr[a].first / block_size, bb = lr[b].first / block_size;
            if (ba != bb) return ba < bb;
            return (ba & 1) ? lr[a].second > lr[b].second : lr[a].second < lr[b].second;
        });
        int l = 0, r = 0;
        for(auto i : ord) {
            while(l > lr[i].first) al(--l);   // 左端を拡張
            while(r < lr[i].second) ar(r++);  // 右端を拡張
            while(l < lr[i].first) el(l++);   // 左端を縮小
            while(r > lr[i].second) er(--r);  // 右端を縮小
            out(i);
        }
    }
};