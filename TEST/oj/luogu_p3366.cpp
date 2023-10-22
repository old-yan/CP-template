#include "GRAPH/Kruskal.h"
#include "GRAPH/Boruvka.h"
#include "GRAPH/Prim_heap.h"
#include "GRAPH/Prim_naive.h"
#include "IO/FastIO.h"

/*
[P3366 【模板】最小生成树](https://www.luogu.com.cn/problem/P3366)
*/
/**
 * 本题为最小生成树模板题
 */
static constexpr uint32_t N = 5000, M = 200000;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::Kruskal::Graph<uint32_t, N, M> G(n, m);
    // OY::Boruvka::Graph<uint32_t, N, M> G(n, m);
    // OY::PrimHeap::Graph<uint32_t, N, M> G(n, m);
    // OY::PrimNaive::Graph<uint32_t, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cost;
        cin >> a >> b >> cost;
        G.add_edge(a - 1, b - 1, cost);
    }

    auto res = G.calc<false>();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag)
        cout << "orz";
    else
        cout << sol.total_cost();
}