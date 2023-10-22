#include "GRAPH/Bipartite.h"
#include "IO/FastIO.h"

/*
[U248878 染色法判定二分图](https://www.luogu.com.cn/problem/U248878)
*/
/**
 * 本题为二分图判定模板题
*/
static constexpr uint32_t N = 100000, M = 100000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Bipartite::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }
    cout << (G.calc().is_bipartite() ? "Yes" : "No");
}
