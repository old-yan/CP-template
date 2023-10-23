#include "GRAPH/Edmonds_tarjan.h"
#include "IO/FastIO.h"

/*
[U210116 【模板】最小树形图（加强版）](https://www.luogu.com.cn/problem/U210116)
*/
/**
 * 本题为最小树形图模板题
 */
static constexpr uint32_t N = 100000, M = 1000000;
int main() {
    uint32_t n, m, r;
    cin >> n >> m >> r;
    OY::EdmondsTarjan::Graph<uint64_t, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cost;
        cin >> a >> b >> cost;
        G.add_edge(a - 1, b - 1, cost);
    }
    auto res = G.calc<false>(r - 1);
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag)
        cout << "-1\n";
    else
        cout << sol.total_cost();
}