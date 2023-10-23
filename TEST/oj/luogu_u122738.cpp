#include "GRAPH/Edmonds_tarjan.h"
#include "IO/FastIO.h"

/*
[U122738 【模板】最小树形图 2](https://www.luogu.com.cn/problem/U122738)
*/
/**
 * 本题为最小树形图输出方案模板题，且只能使用堆优化版本
 */
static constexpr uint32_t N = 300000, M = 1000000;
int main() {
    uint32_t n, m, r;
    cin >> n >> m >> r;
    OY::EdmondsTarjan::Graph<uint64_t, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cost;
        cin >> a >> b >> cost;
        G.add_edge(a - 1, b - 1, cost);
    }
    auto res = G.calc<true>(r - 1);
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag)
        cout << "-1\n";
    else {
        cout << sol.total_cost() << endl;
        sol.do_for_used_edges([](uint32_t index) {
            cout << index + 1 << ' ';
        });
    }
}