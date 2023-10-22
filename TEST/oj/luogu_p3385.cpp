#include "GRAPH/BellmanFord.h"
#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P3385 【模板】负环](https://www.luogu.com.cn/problem/P3385)
*/
/**
 * 本题为 Bellman-Ford 算法判负环模板题
 *  SPFA 为 Bellman-Ford 算法的上位替代
 */
static constexpr uint32_t N = 20000, M = 60000;
int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        OY::BellmanFord::Graph<int, N, M * 2> G(n, m * 2);
        // OY::SPFA::Graph<int, N, M * 2> G(n, m * 2);
        for (uint32_t i = 0; i < m; i++) {
            uint32_t a, b;
            int dis;
            cin >> a >> b >> dis;
            G.add_edge(a - 1, b - 1, dis);
            if (dis >= 0) G.add_edge(b - 1, a - 1, dis);
        }

        if (G.has_negative_cycle(0))
            cout << "YES\n";
        else
            cout << "NO\n";
    }
}