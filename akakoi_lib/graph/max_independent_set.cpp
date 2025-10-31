unsigned long long MaxIndependentSet(vector<vector<int>> g) {
   using U = unsigned long long;
   int n = g.size();
   vector<U> nbd(n);
   for(int i = 0; i < n; ++i) {
      for(auto e : g[i]) { nbd[i] |= 1ULL << e; }
   }
   int best = 0;
   U res = 0;
   auto dfs = [&](auto&& dfs, U now, U rest) -> void {
      pair<int, int> p(-1, -1);
      while(true) {
         bool upd = false;
         for(int v = 0; v < n; ++v) {
            if(rest >> v & 1) {
               int d = __builtin_popcount(nbd[v] & rest);
               if(d > p.second) {
                  p.second = d;
                  p.first = v;
               }
               if(d <= 1) {
                  rest ^= 1ULL << v;
                  rest &= ~nbd[v];
                  now |= 1ULL << v;
                  upd = true;
               }
            }
         }
         if(!upd) break;
         p = {-1, -1};
      }
      int a = __builtin_popcount(now), b = __builtin_popcount(rest);
      if(a > best) {
         best = a;
         res = now;
      }
      if(b == 0 || a + b <= best) return;
      int v = p.first;
      rest &= ~(1ULL << v);
      if(p.second >= 3) dfs(dfs, now, rest);
      now |= 1ULL << v;
      dfs(dfs, now, rest & ~(nbd[v]));
   };
   U now = 0, rest = (1ULL << n) - 1;
   dfs(dfs, now, rest);
   return res;
}
