struct MCF {
    struct edge {
        int to;
        ll cap, cost;
        int rev;
        bool isrev;
        edge(int to, ll cap, ll cost, int rev, bool isrev)
            : to(to), cap(cap), cost(cost), rev(rev), isrev(isrev) {}
    };
    vector<vector<edge>> g;
    vector<ll> pot, cost;
    vector<int> pv, pe;
    MCF(int n) : g(n) {}
    void add(int u, int v, ll cap, ll cst) {
        g[u].push_back(edge(v, cap, cst, (int)g[v].size(), false));
        g[v].push_back(edge(u, 0, -cst, (int)g[u].size() - 1, true));
    }
    ll mcf(int s, int t, ll f) {
        int n = (int)g.size();
        ll ret = 0;
        const ll INFL = numeric_limits<ll>::max();
        pot.assign(n, 0);
        pe.assign(n, -1);
        pv.assign(n, -1);
        using P = pair<ll, int>;
        priority_queue<P, vector<P>, greater<>> pq;
        while (f > 0) {
            cost.assign(n, INFL);
            cost[s] = 0;
            pq.emplace(0, s);
            while (!pq.empty()) {
                auto [c, x] = pq.top();
                pq.pop();
                if (cost[x] < c) continue;
                for (int i = 0; i < (int)g[x].size(); i++) {
                    edge &e = g[x][i];
                    if (e.cap > 0) {
                        ll ncost = cost[x] + e.cost + pot[x] - pot[e.to];
                        if (ncost < cost[e.to]) {
                            cost[e.to] = ncost;
                            pv[e.to] = x;
                            pe[e.to] = i;
                            pq.emplace(cost[e.to], e.to);
                        }
                    }
                }
            }
            if (cost[t] == INFL) return -1; // 流せない
            for (int i = 0; i < n; i++) pot[i] += cost[i];
            ll addflow = f;
            for (int v = t; v != s; v = pv[v]) {
                addflow = min(addflow, g[pv[v]][pe[v]].cap);
            }
            f -= addflow;
            ret += addflow * pot[t];
            for (int v = t; v != s; v = pv[v]) {
                auto &e = g[pv[v]][pe[v]];
                e.cap -= addflow;
                g[v][e.rev].cap += addflow;
            }
        }
        return ret;
    }
};