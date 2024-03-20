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
    OY::SCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }
    auto sol = G.calc();
    uint32_t ans = 0;
    for (auto &&e : sol.get_groups())
        if (e.size() > 1) ans++;
    cout << ans;
}