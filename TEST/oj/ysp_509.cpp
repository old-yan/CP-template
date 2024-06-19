#include "GRAPH/KthPath.h"
#include "IO/FastIO.h"

/*
[K-Shortest Walk](https://judge.yosupo.jp/problem/k_shortest_walk)(https://github.com/yosupo06/library-checker-problems/issues/509)
*/
/**
 * 本题为 K 短路模板题
 * 注意，本题允许反复路过终点
 */

static constexpr uint32_t N = 300000, M = 300000;
int main() {
    uint32_t n, m, s, t, k;
    cin >> n >> m >> s >> t >> k;
    OY::KPATH::Graph<uint64_t, true, N, M> G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        G.add_edge(a, b, c);
    }
    G.calc(s, t);
    uint32_t cur = 0;
    for (uint32_t i = 0; i != k; i++) {
        auto res = G.next();
        if (res < UINT64_MAX / 4)
            cout << res << endl;
        else {
            while (i != k) cout << "-1\n", i++;
            break;
        }
    }
}