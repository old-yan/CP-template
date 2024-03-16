#include "GRAPH/Dijkstra_heap.h"
#include "IO/FastIO.h"

/*
[#622. 单源最短路径](https://uoj.ac/problem/622)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */
static constexpr uint32_t N = 300000, M = 1000000;
int main() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::DijkstraHeap::Graph<uint64_t, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto res = G.calc<false>(s - 1);
    for (uint32_t i = 0; i < n; i++) {
        auto x = res.query(i);
        if (x < INT64_MAX)
            cout << x << ' ';
        else
            cout << "-1\n";
    }
}
