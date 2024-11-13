#include "DS/LinkBucketHeap.h"
#include "GRAPH/BellmanFord.h"
#include "GRAPH/Dijkstra.h"
#include "GRAPH/Dijkstra_naive.h"
#include "GRAPH/SPFA.h"
#include "IO/FastIO.h"

/*
[Deleting Edges](https://acm.hdu.edu.cn/showproblem.php?pid=6026)
*/
/**
 * 本题为 Dijkstra 算法模板题
 * 由于最终生成的树，从 0 到每个结点的距离都是原来的最短距离
 * 那么可以枚举每个结点的前驱，找出可能的最短路前驱，再随便选一条
 */

char dis[50][50];
void solve_dijk_naive() {
    uint32_t n;
    while (cin >> n) {
        using monoid = OY::DijkstraNaive::AddGroup<uint32_t>;
        OY::DijkstraNaive::Graph<monoid> G(n);
        for (uint32_t i = 0; i != n; i++)
            for (uint32_t j = 0; j != n; j++) cin >> dis[i][j];
        for (uint32_t i = 0; i != n; i++)
            for (uint32_t j = 0; j != n; j++)
                if (dis[i][j] != '0') G.add_edge(i, j, dis[i][j] - '0');

        auto sol = G.calc(0);
        uint32_t ans = 1;
        for (uint32_t i = 1; i != n; i++) {
            uint32_t cand = 0;
            for (uint32_t j = 0; j != n; j++)
                if (dis[i][j] != '0')
                    if (sol.query(j) + (dis[j][i] - '0') == sol.query(i)) cand++;
            ans = 1ull * ans * cand % 1000000007;
        }
        cout << ans << endl;
    }
}

void solve_dijk() {
    uint32_t n;
    while (cin >> n) {
        OY::Dijkstra::Graph<uint32_t> G(n, n * n);
        for (uint32_t i = 0; i != n; i++)
            for (uint32_t j = 0; j != n; j++) cin >> dis[i][j];
        for (uint32_t i = 0; i != n; i++)
            for (uint32_t j = 0; j != n; j++)
                if (dis[i][j] != '0') G.add_edge(i, j, dis[i][j] - '0');

        // 采用桶堆，优化时间复杂度到线性
        using monoid = OY::Dijkstra::AddGroup<uint32_t>;
        auto sol = G.calc<monoid, void, false, OY::LBHeap::HeapWrap<10>::type>(0);
        uint32_t ans = 1;
        for (uint32_t i = 1; i != n; i++) {
            uint32_t cand = 0;
            for (uint32_t j = 0; j != n; j++)
                if (dis[i][j] != '0')
                    if (sol.query(j) + (dis[j][i] - '0') == sol.query(i)) cand++;
            ans = 1ull * ans * cand % 1000000007;
        }
        cout << ans << endl;
    }
}

void solve_spfa() {
    uint32_t n;
    while (cin >> n) {
        // OY::SPFA::Graph<uint32_t> G(n, n * n);
        OY::BellmanFord::Graph<uint32_t> G(n, n * n);
        for (uint32_t i = 0; i != n; i++)
            for (uint32_t j = 0; j != n; j++) cin >> dis[i][j];
        for (uint32_t i = 0; i != n; i++)
            for (uint32_t j = 0; j != n; j++)
                if (dis[i][j] != '0') G.add_edge(i, j, dis[i][j] - '0');

        auto [sol, flag] = G.calc(0);
        uint32_t ans = 1;
        for (uint32_t i = 1; i != n; i++) {
            uint32_t cand = 0;
            for (uint32_t j = 0; j != n; j++)
                if (dis[i][j] != '0')
                    if (sol.query(j) + (dis[j][i] - '0') == sol.query(i)) cand++;
            ans = 1ull * ans * cand % 1000000007;
        }
        cout << ans << endl;
    }
}

int main() {
    solve_dijk_naive();
    // solve_dijk();
    // solve_spfa();
}