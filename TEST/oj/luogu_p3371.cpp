#include "GRAPH/Dijkstra_heap.h"
#include "GRAPH/Dijkstra_naive.h"
#include "GRAPH/SPFA.h"
#include "GRAPH/BellmanFord.h"
#include "IO/FastIO.h"

/*
[P3371 【模板】单源最短路径（弱化版）](https://www.luogu.com.cn/problem/P3371)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */
static constexpr uint32_t N = 10000, M = 500000;
int main() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::DijkstraHeap::Graph<uint32_t, N, M> G(n, m);
    // OY::DijkstraNaive::Graph<uint32_t, N, M> G(n, m);
    // OY::SPFA::Graph<uint32_t, N, M> G(n, m);
    // OY::BellmanFord::Graph<uint32_t, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto res = G.calc<false>(s - 1);
    // 当使用 SPFA 或者 BellmanFord 方法时，因为 calc 返回的是 pair，所以需要加 first
    // auto res = G.calc<false>(s - 1).first;
    for (uint32_t i = 0; i < n; i++) cout << res.query(i) << ' ';
}
