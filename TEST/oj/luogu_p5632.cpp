#include "GRAPH/StoerWagner.h"
#include "IO/FastIO.h"

/*
[P5632 【模板】Stoer-Wagner](https://www.luogu.com.cn/problem/P5632)
*/
/**
 * 本题为全局最小割模板题
 */
static constexpr uint32_t N = 600, M = 300 * 599;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    // 由于本模板在计算时会修改边权，所以如果要记录方案，就必须在外部保存边的副本
    OY::StoerWagner::Graph<uint32_t> G(n);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cost;
        cin >> a >> b >> cost;
        G.add_edge(a - 1, b - 1, cost);
    }
    cout << G.calc<false>();
}