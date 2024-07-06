#include "GRAPH/Dinic.h"
#include "GRAPH/EdmondsKarp.h"
#include "IO/FastIO.h"

/*
[P3376 【模板】网络最大流](https://www.luogu.com.cn/problem/P3376)
*/
/**
 * 本题为最大流模板题
 */

int main() {
    uint32_t n, m, s, t;
    cin >> n >> m >> s >> t;

    OY::DINIC::Graph<int> G(n, m);
    // OY::EK::Graph<int> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cap;
        cin >> a >> b >> cap;
        G.add_edge(a - 1, b - 1, cap);
    }

    auto res = G.calc<uint64_t>(s - 1, t - 1);
    cout << res;
}
