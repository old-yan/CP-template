#include "GRAPH/BellmanFord.h"
#include "GRAPH/Dijkstra_heap.h"
#include "GRAPH/Dijkstra_naive.h"
#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P3371 【模板】单源最短路径（弱化版）](https://www.luogu.com.cn/problem/P3371)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */

void solve_bf() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::BellmanFord::Graph<uint32_t> G(n, m);
    // OY::SPFA::Graph<uint32_t> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto res = G.calc(s - 1).first;
    for (uint32_t i = 0; i < n; i++) cout << res.query(i) << ' ';
}

void solve_dijk() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::DijkstraHeap::Graph<uint32_t> G(n, m);
    // OY::DijkstraNaive::Graph<uint32_t> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto sol = G.calc(s - 1);
    for (uint32_t i = 0; i < n; i++) cout << (sol.query_count(i) ? sol.query(i) : 2147483647) << ' ';
}

int main() {
    solve_bf();
    // solve_dijk();
}