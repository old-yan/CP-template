#include "GRAPH/Psuedotree_ug.h"
#include "DS/GlobalUnorderedDiscretizer.h"
#include "IO/FastIO.h"

#include <array>

/*
[#2774. 「BalticOI 2018」多角恋](https://loj.ac/p/2774)
*/
/**
 * 在基环森林里选最大匹配
 * 与 P1453 类似，P1453 是选择最多的不相邻点，而本题为选择最多的无交边
 */

static constexpr uint32_t N = 100000;
OY::GlobalUnorderedDiscretizer<std::string, 130007> GD;
uint32_t dp[N][2];
int main() {
    uint32_t n;
    cin >> n;
    if (n & 1) {
        cout << "-1";
        return 0;
    }
    OY::PsuedoUG::Graph<bool> G(n);
    for (uint32_t i = 0; i != n; i++) {
        std::string a, b;
        cin >> a >> b;
        auto ai = GD.insert(a), bi = GD.insert(b);
        G.add_edge(ai, bi);
    }
    G.prepare();

    // 对环外部分进行树形 dp
    for (uint32_t i = 0; i != n; i++)
        if (!G.get_vertex_info(i).step_to_cycle()) {
            auto dfs = [&](auto &&self, uint32_t a) -> void {
                bool same = false;
                G.do_for_out_edges(a, [&](uint32_t to) {
                    self(self, to);
                    dp[a][0] += dp[to][1];
                    same |= dp[to][0] == dp[to][1];
                });
                dp[a][1] = same ? dp[a][0] + 1 : dp[a][0];
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
    uint32_t ans = 0;
    for (uint32_t id = 0; id != G.cycle_count(); id++) {
        auto cycle = G.get_cycle_info(id);
        uint32_t len = cycle.size();
        if (len == 1)
            ans += dp[cycle[0]][1];
        else if (len == 2)
            ans += dp[cycle[0]][0] + dp[cycle[1]][0] + 2;
        else {
            std::array<std::array<uint64_t, 2>, 2> roll_dp{};
            // 直接钦定出在 cycle[1] 处的 dp 值
            roll_dp[0][0] = dp[cycle[0]][0] + dp[cycle[1]][0];
            roll_dp[0][1] = dp[cycle[0]][0] + dp[cycle[1]][1];
            roll_dp[1][0] = dp[cycle[0]][1] + dp[cycle[1]][0];
            roll_dp[1][1] = std::max(dp[cycle[0]][0] + dp[cycle[1]][0] + 1, dp[cycle[0]][1] + dp[cycle[1]][1]);
            for (uint32_t idx = 2; idx != len; idx++) {
                uint32_t i = cycle[idx];
                std::array<std::array<uint64_t, 2>, 2> roll_dp2;
                roll_dp2[0][0] = roll_dp[0][1] + dp[i][0];
                roll_dp2[0][1] = std::max(roll_dp[0][0] + dp[i][0] + 1, roll_dp[0][1] + dp[i][1]);
                roll_dp2[1][0] = roll_dp[1][1] + dp[i][0];
                roll_dp2[1][1] = std::max(roll_dp[1][0] + dp[i][0] + 1, roll_dp[1][1] + dp[i][1]);
                roll_dp.swap(roll_dp2);
            }
            ans += std::max(roll_dp[0][0] + 1, roll_dp[1][1]);
        }
    }
    cout << n - ans;
}
