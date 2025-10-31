template<typename T>
struct Dinic {
    const T INF;
    struct edge {
        int to;
        T cap;
        int rev;
        bool isrev;
        int idx;
    };
    vector<vector<edge>> g;
    vector<int> c, iter;
    Dinic(int V) : INF(numeric_limits<T>::max()), g(V) {}
    void add_edge(int from, int to, T cap, int idx = -1) {
        g[from].push_back(edge{to, cap, (int)g[to].size(), false, idx});
        g[to].push_back(edge{from, 0, (int)g[from].size() - 1, true, idx});
    }
    bool bfs(int s, int t) {
        c.assign((int)g.size(), -1);
        queue<int> q;
        c[s] = 0;
        q.push(s);
        while (!q.empty() && c[t] == -1) {
            int x = q.front();
            q.pop();
            for (auto &e : g[x]) {
                if (e.cap > 0 && c[e.to] == -1) {
                    c[e.to] = c[x] + 1;
                    q.push(e.to);
                }
            }
        }
        return c[t] != -1;
    }
    T dfs(int x, int t, T flow) {
        if (x == t) return flow;
        for (int &i = iter[x]; i < (int)g[x].size(); i++) {
            edge &e = g[x][i];
            if (e.cap > 0 && c[x] < c[e.to]) {
                T d = dfs(e.to, t, min(flow, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    T max_flow(int s, int t) {
        T flow = 0;
        while (bfs(s, t)) {
            iter.assign((int)g.size(), 0);
            T f;
            while ((f = dfs(s, t, INF)) > 0) {
                flow += f;
            }
        }
        return flow;
    }
};