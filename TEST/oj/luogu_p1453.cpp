#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"

#include <array>

/*
[P1453 城市环路](https://www.luogu.com.cn/problem/P1453)
*/
/**
 * 本题为基环树模板题，同样为求带权最大独立集
 * 与 P2607 的区别是，本题为无向图，所以需要定向之后再求
 */

static constexpr uint32_t N = 100000;
uint32_t w[N];
uint32_t dp[N][2];
int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) cin >> w[i];
    OY::PsuedoUG::Graph<bool> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a, b);
    }
    G.prepare();

    // 对环外部分进行树形 dp
    for (uint32_t i = 0; i != n; i++)
        if (!G.get_vertex_info(i).step_to_cycle()) {
            auto dfs = [&](auto &&self, uint32_t a) -> void {
                G.do_for_out_edges(a, [&](uint32_t to) {
                    self(self, to);
                    dp[a][0] += std::max(dp[to][0], dp[to][1] + w[to]);
                    dp[a][1] += dp[to][0];
                });
            };
            dfs(dfs, i);
        }

    // 此时，对于环上的交点，dp[x][0] 表示不选 x 的环外收益，dp[x][1] 表示选 x 的环外收益
    // 在环上进行环上的 dp
    // dp[0][0] 表示不选头，不选 x 的总收益
    // dp[0][1] 表示不选头，选 x 的总收益
    // dp[1][0] 表示选头，不选 x 的总收益
    // dp[1][1] 表示选头，选 x 的总收益
    // 环上 dp 可以用滚动数组优化
    uint64_t ans = 0;
    for (uint32_t id = 0; id != G.cycle_count(); id++) {
        auto cycle = G.get_cycle_info(id);
        uint32_t len = cycle.size();
        // 题目已经保证不会有自环，所以 len 不会是 1
        std::array<std::array<uint64_t, 2>, 2> roll_dp{};
        roll_dp[0][0] = dp[cycle[0]][0], roll_dp[1][1] = w[cycle[0]] + dp[cycle[0]][1];
        for (uint32_t idx = 1; idx != len; idx++) {
            uint32_t i = cycle[idx];
            std::array<std::array<uint64_t, 2>, 2> roll_dp2;
            roll_dp2[0][0] = std::max(roll_dp[0][0], roll_dp[0][1]) + dp[i][0];
            roll_dp2[0][1] = roll_dp[0][0] + w[i] + dp[i][1];
            roll_dp2[1][0] = std::max(roll_dp[1][0], roll_dp[1][1]) + dp[i][0];
            roll_dp2[1][1] = roll_dp[1][0] + w[i] + dp[i][1];
            roll_dp.swap(roll_dp2);
        }
        ans += std::max({roll_dp[0][0], roll_dp[0][1], roll_dp[1][0]});
    }
    std::string k;
    cin >> k;
    char out_buf[20]{};
    sprintf(out_buf, "%.1lf", ans * std::stod(k));
    cout << out_buf;
}