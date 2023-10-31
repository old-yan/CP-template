#include "GRAPH/Dinic_mcmf.h"
#include "GRAPH/EdmondsKarp_mcmf.h"
#include "IO/FastIO.h"

/*
[P3381 【模板】最小费用最大流](https://www.luogu.com.cn/problem/P3381)
*/
/**
 * 本题为最小费用最大流模板题
 */
static constexpr uint32_t N = 5000, M = 50000;
int main() {
    uint32_t n, m, s, t;
    cin >> n >> m >> s >> t;
    OY::DINICMCMF::Graph<uint32_t, uint32_t, false, N, M> G(n, m);
    // OY::EKMCMF::Graph<uint32_t, uint32_t, false, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cap, cost;
        cin >> a >> b >> cap >> cost;
        G.add_edge(a - 1, b - 1, cap, cost);
    }
    auto res = G.calc(s - 1, t - 1);
    cout << res.first << ' ' << res.second << '\n';
}