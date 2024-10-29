#include "GRAPH/BFS.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"

/*
[P1144 最短路计数](https://www.luogu.com.cn/problem/P1144)
*/
/**
 * 本题为 Dijkstra 算法模板题
 * 需要计数
 */

void solve_bfs() {
    using mint = OY::StaticModInt32<100003, false>;
    uint32_t n, m;
    cin >> n >> m;
    OY::BFS::Graph G(n, m * 2);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
        G.add_edge(b - 1, a - 1);
    }
    auto sol = G.calc<mint, false>(0);
    for (uint32_t i = 0; i != n; i++) cout << sol.query_count(i) << endl;
}

int main() {
    solve_bfs();
}
