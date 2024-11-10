#include "GRAPH/DijkstraKPath.h"
#include "GRAPH/SPFAKPath.h"
#include "IO/FastIO.h"

/*
[P2865 [USACO06NOV] Roadblocks G](https://www.luogu.com.cn/problem/P2865)
 */
/**
 * 严格次短路模板题
 */

void solve_dijk() {
    uint32_t n, m;
    cin >> n >> m;
    OY::DijkstraKPath::Graph<uint32_t> G(n, m * 2);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, w;
        cin >> a >> b >> w;
        G.add_edge(a - 1, b - 1, w);
        G.add_edge(b - 1, a - 1, w);
    }

    auto sol = G.calc<2>(0, n - 1);
    cout << sol.query(n - 1, 1);
}

void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    OY::SPFAKPath::Graph<uint32_t> G(n, m * 2);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, w;
        cin >> a >> b >> w;
        G.add_edge(a - 1, b - 1, w);
        G.add_edge(b - 1, a - 1, w);
    }

    auto [sol, flag] = G.calc<2>(0);
    cout << sol.query(n - 1, 1);
}

int main() {
    solve_dijk();
    // solve_spfa();
}