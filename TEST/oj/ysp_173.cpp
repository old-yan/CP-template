#include "GRAPH/Dijkstra_heap.h"
#include "IO/FastIO.h"

/*
[Shortest Path](https://judge.yosupo.jp/problem/shortest_path)(https://github.com/yosupo06/library-checker-problems/issues/173)
*/
/**
 * 本题为最短路模板题
 */

struct dist {
    uint64_t m_val;
    uint32_t m_step;
    bool operator>(const dist &rhs) const { return m_val > rhs.m_val; }
    bool operator==(const dist &rhs) const { return m_val == rhs.m_val; }
    dist operator+(uint32_t x) const { return {m_val + x, m_step + 1}; }
};
int main() {
    uint32_t n, m, s, t;
    cin >> n >> m >> s >> t;
    OY::DijkstraHeap::Graph<uint32_t> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a, b, c);
    }
    auto sol = G.calc<true, dist>(s, t, {0x3f3f3f3f3f3f3f3f});
    if (sol.query(t) == sol.m_infinite) {
        cout << "-1\n";
        return 0;
    }
    cout << sol.query(t).m_val << ' ' << sol.query(t).m_step << '\n';
    sol.trace(t, [&](uint32_t from, uint32_t to) {
        cout << from << ' ' << to << endl;
    });
}