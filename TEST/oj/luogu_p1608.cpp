#include "GRAPH/BellmanFord.h"
#include "GRAPH/Dijkstra_heap.h"
#include "GRAPH/Dijkstra_naive.h"
#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[P1608 路径统计](https://www.luogu.com.cn/problem/P1608)
*/
/**
 * 本题为 Dijkstra 算法模板题
 * 需要计数
 */

uint32_t g[2000][2000];
auto update = [](auto &x, auto y) {
    if (!x)
        x = y;
    else
        x = std::min(x, y);
};
void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        update(g[a - 1][b - 1], dis);
    }

    OY::SPFA::Graph<uint32_t> G(n, std::min(n * n, m));
    // OY::BellmanFord::Graph<uint32_t>G(n, std::min(n * n, m));
    for (uint32_t i = 0; i < n; i++)
        for (uint32_t j = 0; j < n; j++)
            if (g[i][j])
                G.add_edge(i, j, g[i][j]);

    using monoid = OY::SPFA::AddSemiGroup<uint32_t>;
    // using monoid = OY::BellmanFord::AddSemiGroup<uint32_t>;

    auto [sol, flag] = G.calc<monoid, uint32_t>(0);
    if (sol.query_count(n - 1))
        cout << sol.query(n - 1) << ' ' << sol.query_count(n - 1);
    else
        cout << "No answer";
}

void solve_dijk() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        update(g[a - 1][b - 1], dis);
    }

    OY::DijkstraHeap::Graph<uint32_t> G(n, std::min(n * n, m));
    // OY::DijkstraNaive::Graph<uint32_t> G(n, std::min(n * n, m));
    for (uint32_t i = 0; i < n; i++)
        for (uint32_t j = 0; j < n; j++)
            if (g[i][j])
                G.add_edge(i, j, g[i][j]);

    using monoid = OY::DijkstraHeap::AddSemiGroup<uint32_t>;
    // using monoid = OY::DijkstraNaive::AddSemiGroup<uint32_t>;

    auto sol = G.calc<monoid, uint32_t>(0);
    if (sol.query_count(n - 1))
        cout << sol.query(n - 1) << ' ' << sol.query_count(n - 1);
    else
        cout << "No answer";
}

int main() {
    solve_spfa();
    // solve_dijk();
}
