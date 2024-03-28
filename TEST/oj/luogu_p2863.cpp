#include "GRAPH/Tarjan_scc.h"
#include "IO/FastIO.h"

/*
[P2863 [USACO06JAN] The Cow Prom S](https://www.luogu.com.cn/problem/P2863)
*/
/**
 * 本题求强连通分量
 */

static constexpr uint32_t N = 10000, M = 50000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SCC::Graph G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }
    auto sol = G.calc();
    // 找出每个同号的区间
    uint32_t ans = 0;
    for (uint32_t i = 0, j; i != n; i = j) {
        uint32_t id = sol.query(sol.m_topo[i]);
        for (j = i + 1; j != n && sol.query(sol.m_topo[j]) == id; j++) {}
        ans += j > i + 1;
    }
    cout << ans;
}