#include "GRAPH/DynamicSPFA.h"
#include "IO/FastIO.h"

/*
[P2387 [NOI2014] 魔法森林](https://www.luogu.com.cn/problem/P2387)
*/
/**
 * 本题可以在 a 单调的情况下追加边，贪心求最小 b
 */

void solve_spfa() {
    uint32_t n, m;
    cin >> n >> m;
    struct Edge {
        uint32_t u, v, a, b;
    };
    std::vector<Edge> es(m);
    for (auto &[u, v, a, b] : es) cin >> u >> v >> a >> b, u--, v--;
    std::ranges::sort(es, [](auto &x, auto &y) {
        if (x.a != y.a) return x.a < y.a;
        return x.b < y.b;
    });

    // 假设带了 x 个 A 型
    // 那么 a<=x 的边皆可用
    // 尽量使用较小的 b
    // 令路权为边权最大值，求最短路
    OY::StaticDynamicSPFA<OY::DSPFA::MaxGroup<uint32_t>, false, 50000, 200000> G(n, m * 2);
    G.set_distance(0, 0);
    uint32_t ans = -1;
    for (auto &[u, v, a, b] : es) {
        G.update_by_edge(u, v, b), G.update_by_edge(v, u, b);
        if (G.query(n - 1) != G.infinite()) ans = std::min(ans, a + G.query(n - 1));
    }
    cout << int(ans);
}

int main() {
    solve_spfa();
}