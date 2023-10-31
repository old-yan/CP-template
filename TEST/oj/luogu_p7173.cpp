#include "GRAPH/Dinic_mcmf.h"
#include "GRAPH/EdmondsKarp_mcmf.h"
#include "IO/FastIO.h"

/*
[P7173 【模板】有负圈的费用流](https://www.luogu.com.cn/problem/P7173)
*/
/**
 * 本题为带负圈的最小费用最大流模板题
 */
static constexpr uint32_t N = 200, M = 10000;
int main() {
    uint32_t n, m, s, t;
    cin >> n >> m >> s >> t;
    OY::DINICMCMF::NegativeCycleGraph<int, int, N, M> G(n, m);
    // OY::EKMCMF::NegativeCycleGraph<int, int, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        int from, to, flow, cost;
        cin >> from >> to >> flow >> cost;
        G.add_edge(from - 1, to - 1, flow, cost);
    }
    G.set(s - 1, t - 1);
    auto [flow, cost] = G.calc();
    cout << flow << ' ' << cost;
}
