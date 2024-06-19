#include "GRAPH/Dijkstra_heap.h"
#include "IO/FastIO.h"

/*
[Shortest Path](https://judge.yosupo.jp/problem/shortest_path)(https://github.com/yosupo06/library-checker-problems/issues/173)
*/
/**
 * 本题为最短路模板题
 */

static constexpr uint32_t N = 500000, M = 500000;
int main() {
    uint32_t n, m, s, t;
    cin >> n >> m >> s >> t;
    OY::DijkstraHeap::Graph<uint64_t, N, M> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a, b, c);
    }
    auto sol = G.calc<true>(s);
    if (sol.query(t) == sol.m_infinite) {
        cout << "-1\n";
        return 0;
    }
    uint32_t step = 0;
    sol.trace(t, [&](auto...) { step++; });
    cout << sol.query(t) << ' ' << step << endl;
    sol.trace(t, [&](uint32_t from, uint32_t to) {
        cout << from << ' ' << to << endl;
    });
}