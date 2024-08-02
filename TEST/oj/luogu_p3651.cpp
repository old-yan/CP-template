#include "GRAPH/Psuedotree_ig.h"
#include "IO/FastIO.h"

#include <cstdint>
#include <numeric>

/*
[P3651 展翅翱翔之时 (はばたきのとき)](https://www.luogu.com.cn/problem/P3651)
*/
/**
 * 本题为内向基环树模板题
 * 要想让所有结点互相可达，必须形成环
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::PsuedoIG::Graph<uint32_t, uint32_t, true> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t to, cost;
        cin >> to >> cost;
        G.set_edge(i, to - 1, cost);
    }
    G.prepare();

    if (G.cycle_count() == 1 && G.get_cycle_info(0).size() == n) {
        // 初始就是环，那么不需要动
        cout << 0;
    } else {
        uint64_t ans = 0;
        for (uint32_t id = 0; id != G.cycle_count(); id++) {
            auto cycle = G.get_cycle_info(id);
            // 记录环上的每个点，把环外部分撕成链需要多少代价。也就是 cycle[i] 在环外的所有非最长边的和
            uint64_t drop_sum = 0;
            // 记录环上的每个点，环外的直接儿子的最长边
            std::vector<uint32_t> longest_off_cycle(cycle.size());
            for (uint32_t i = 0; i != cycle.size(); i++) {
                uint32_t v = cycle[i];
                auto dfs = [&](auto &&self, uint32_t a) -> void {
                    uint64_t sum = 0;
                    uint32_t max = 0;
                    G.do_for_out_edges(a, [&](uint32_t to, uint32_t dis) {
                        sum += dis;
                        max = std::max(max, dis);
                        if (a == v) longest_off_cycle[i] = std::max(longest_off_cycle[i], dis);
                        self(self, to);
                    });
                    drop_sum += sum - max;
                };
                dfs(dfs, v);
            }
            // 先假定环上的每条边都不拆
            ans += drop_sum + std::accumulate(longest_off_cycle.begin(), longest_off_cycle.end(), uint64_t());
            // 检查哪些环上的边可以拆：环上 cycle[i] 到 cycle[i+1] 的边拆掉之后，cycle[i+1] 和环外的最长边可以保留不拆
            // 只要能拆，就把贡献算上
            // 如果一条都拆不了，选一条亏损最小的
            bool chosen = false;
            uint32_t min_bad = 0x3f3f3f3f;
            for (uint32_t i = 0; i != cycle.size(); i++) {
                uint32_t len_on_cycle = i + 1 == cycle.size() ? cycle.length() - cycle.position(i) : cycle.position(i + 1) - cycle.position(i);
                uint32_t len_off_cycle = longest_off_cycle[i + 1 == cycle.size() ? 0 : i + 1];
                if (len_on_cycle <= len_off_cycle) {
                    ans -= len_off_cycle - len_on_cycle;
                    chosen = true;
                } else
                    min_bad = std::min(min_bad, len_on_cycle - len_off_cycle);
            }
            if (!chosen) ans += min_bad;
        }
        cout << ans << endl;
    }
}