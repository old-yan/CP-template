#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"

/*
[P4779 【模板】单源最短路径（标准版）](https://www.luogu.com.cn/problem/P4779)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */

int main() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::Dijkstra::Graph<uint32_t> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto sol = G.calc(s - 1);
    for (uint32_t i = 0; i < n; i++) cout << sol.query(i) << ' ';
}
