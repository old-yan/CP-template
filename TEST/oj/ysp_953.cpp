#include "GRAPH/Kruskal.h"
#include "IO/FastIO.h"

/*
[Minimum Spanning Tree](https://judge.yosupo.jp/problem/minimum_spanning_tree)(https://github.com/yosupo06/library-checker-problems/issues/953)
*/
/**
 * 本题为最小生成树模板题
 */

static constexpr uint32_t N = 500000, M = 500000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Kruskal::Graph<uint64_t, N, M> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a, b, c);
    }
    auto sol = G.calc<true>().first;
    cout << sol.total_cost() << endl;
    sol.do_for_used_edges([&](uint32_t index, auto...) {
        cout << index << ' ';
    });
}