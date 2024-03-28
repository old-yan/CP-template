#include "DS/LinkBucket.h"
#include "GRAPH/BFS.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"

/*
[P1144 最短路计数](https://www.luogu.com.cn/problem/P1144)
*/
/**
 * 本题可以先求出最短路
 * 再在最短路所经过的路径上 dp
 */

static constexpr uint32_t N = 1000000, M = 2000000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BFS::Graph<N, M * 2> G(n, m * 2);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        if (a != b)
            G.add_edge(a - 1, b - 1), G.add_edge(b - 1, a - 1);
    }
    auto sol = G.calc<false>(0);

    OY::LBC::LinkBucket<uint32_t, N, N> lbc(n, n);
    for (uint32_t i = 0; i < n; i++)
        if (sol.query(i) < n) lbc[sol.query(i)].push_back(i);

    using mint = OY::StaticModInt32<100003, true>;
    std::vector<mint> dp(n);
    dp[0] = 1;
    for (int d = 0; d < n; d++) {
        for (auto from : lbc[d]) {
            G(from, [&](uint32_t to) {
                if (sol.query(to) == d + 1)
                    dp[to] += dp[from];
            });
        }
    }
    for (auto v : dp) cout << v << endl;
}