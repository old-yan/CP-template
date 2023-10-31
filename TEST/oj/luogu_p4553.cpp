#include "GRAPH/Dinic_mcmf.h"
#include "GRAPH/EdmondsKarp_mcmf.h"
#include "IO/FastIO.h"

/*
[P4553 80人环游世界](https://www.luogu.com.cn/problem/P4553)
*/
/**
 * 本题为有上下界最小费用最大流模板题
 */
static constexpr uint32_t N = 100, M = 10301;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::EKMCMF::BoundGraph<int, int, N * 2 + 3, M> G(n * 2 + 3, n * 3 + n * n + 1);
    // OY::DINICMCMF::BoundGraph<int, int, N * 2 + 3, M> G(n * 2 + 3, n * 3 + n * n + 1);
    // 源点为 n*2，汇点为 n*2+1
    for (uint32_t i = 0; i < n; i++) {
        G.add_edge(n * 2, i * 2, 0, m, 0);
        G.add_edge(i * 2 + 1, n * 2 + 1, 0, m, 0);
    }
    // 通过拆点，限制必须通过结点的人数
    for (uint32_t i = 0; i < n; i++) {
        int x;
        cin >> x;
        G.add_edge(i * 2, i * 2 + 1, x, x, 0);
    }
    // 飞机
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = i + 1; j < n; j++) {
            int x;
            cin >> x;
            if (~x) G.add_edge(i * 2 + 1, j * 2, 0, m, x);
        }
    }
    // 汇点拆点，限制人数
    G.add_edge(n * 2 + 1, n * 2 + 2, 0, m, 0);

    G.set(n * 2, n * 2 + 2);
    auto [flow, cost, flag] = G.is_possible();
    cout << cost;
}
