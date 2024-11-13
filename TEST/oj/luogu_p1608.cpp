#include "DS/LinkBucketHeap.h"
#include "GRAPH/BellmanFord.h"
#include "GRAPH/Dijkstra.h"
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

void solve_dijk_naive() {
    uint32_t n, m;
    cin >> n >> m;

    OY::StaticAddDijkstraNaive<uint32_t, uint32_t, std::less<uint32_t>, 0x7fffffff, 0x7fffffff, 2000> G(n);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
    }

    auto sol = G.calc<uint32_t>(0);
    if (sol.query_count(n - 1))
        cout << sol.query(n - 1) << ' ' << sol.query_count(n - 1);
    else
        cout << "No answer";
}

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

    using monoid = OY::SPFA::AddGroup<uint32_t>;
    // using monoid = OY::BellmanFord::AddGroup<uint32_t>;

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

    OY::Dijkstra::Graph<uint32_t> G(n, std::min(n * n, m));
    for (uint32_t i = 0; i < n; i++)
        for (uint32_t j = 0; j < n; j++)
            if (g[i][j])
                G.add_edge(i, j, g[i][j]);

    // 采用桶堆，优化时间复杂度到线性
    using monoid = OY::Dijkstra::AddGroup<uint32_t>;
    auto sol = G.calc<monoid, uint32_t, false, OY::LBHeap::HeapWrap<10>::type>(0);
    if (sol.query_count(n - 1))
        cout << sol.query(n - 1) << ' ' << sol.query_count(n - 1);
    else
        cout << "No answer";
}

int main() {
    solve_dijk_naive();
    // solve_spfa();
    // solve_dijk();
}
