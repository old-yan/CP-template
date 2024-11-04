#include "GRAPH/Dijkstra_heap.h"
#include "IO/FastIO.h"

/*
[#622. 单源最短路径](https://uoj.ac/problem/622)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */

int main() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    OY::DijkstraHeap::Graph<uint32_t> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    using monoid = OY::DijkstraHeap::AddGroup<uint32_t, uint64_t>;
    auto sol = G.calc<monoid>(s - 1);
    for (uint32_t i = 0; i < n; i++) {
        auto x = sol.query(i);
        if (x < INT64_MAX)
            cout << x << ' ';
        else
            cout << "-1\n";
    }
}
