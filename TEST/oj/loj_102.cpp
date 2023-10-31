#include "GRAPH/Dinic_mcmf.h"
#include "GRAPH/EdmondsKarp_mcmf.h"
#include "IO/FastIO.h"

/*
[#102. 最小费用流](https://loj.ac/p/102)
*/
/**
 * 本题为最小费用最大流模板题
 */
static constexpr uint32_t N = 400, M = 15000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::DINICMCMF::Graph<int, int, false, N, M> G(n, m);
    // OY::EKMCMF::Graph<int, int, false, N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t from, to, cap, cost;
        cin >> from >> to >> cap >> cost;
        G.add_edge(from - 1, to - 1, cap, cost);
    }

    auto [flow, cost] = G.calc(0, n - 1);
    cout << flow << ' ' << cost << endl;
}