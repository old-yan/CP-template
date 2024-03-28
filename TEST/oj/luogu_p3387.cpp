#include "GRAPH/Tarjan_scc.h"
#include "IO/FastIO.h"

/*
[P3387 【模板】缩点](https://www.luogu.com.cn/problem/P3387)
*/
/**
 * 本题为缩点模板题，缩点同时考察 SCC 和拓扑排序。此处拓扑排序模板就不贴了，关键是 SCC
 */
static constexpr uint32_t N = 10000, M = 100000;
uint32_t w[N];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> w[i];

    OY::SCC::Graph G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    auto sol = G.calc();
    // 获取每个 SCC 的成员
    auto groups = sol.get_groups();
    uint32_t ans = 0;

    std::vector<uint32_t> weight_of_group(groups.size());
    for (uint32_t i = groups.size() - 1; ~i; i--) {
        for (uint32_t a : groups[i]) weight_of_group[i] += w[a];

        // 对 SCC 内的每个成员，遍历其后继，以获取从本 SCC 出发所能获得的最大值
        uint32_t cur_max = weight_of_group[i];
        for (uint32_t from : groups[i])
            G(from, [&](uint32_t to) {
                if (sol.query(to) != i) cur_max = std::max(cur_max, weight_of_group[i] + weight_of_group[sol.query(to)]);
            });
        weight_of_group[i] = cur_max;
        ans = std::max(ans, cur_max);
    }
    cout << ans;
}