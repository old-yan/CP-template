#include "DS/LinkBucketHeap.h"
#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"

/*
[P2984 [USACO10FEB] Chocolate Giving S](https://www.luogu.com.cn/problem/P2984)
*/
/**
 * 本题为 Dijkstra 算法模板题
 */

void solve_dijk() {
    uint32_t n, m, b;
    cin >> n >> m >> b;
    OY::Dijkstra::Graph<uint32_t> G(n, m * 2);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, dis;
        cin >> a >> b >> dis;
        G.add_edge(a - 1, b - 1, dis);
        G.add_edge(b - 1, a - 1, dis);
    }

    // 采用桶堆加速
    // 其实和普通堆差不多
    auto sol = G.calc<OY::Dijkstra::AddGroup<uint32_t>, void, false, OY::LBHeap::HeapWrap<2001>::type>(0);
    while (b--) {
        uint32_t a, b;
        cin >> a >> b;
        cout << sol.query(a - 1) + sol.query(b - 1) << endl;
    }
}

int main() {
    solve_dijk();
}