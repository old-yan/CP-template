#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"

/*
[P4381 [IOI2008] Island](https://www.luogu.com.cn/problem/P4381)
*/
/**
 * 这是一个无向基环树
 * 可以当作是基环树直径问题
 * 题意为在每个连通分量内找到一条最长链，然后求和
 * 最长链有两种可能：一种是环外的树边组成，一种是环上的圆弧加圆弧两端的树边组成
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::PsuedoUG::Graph<uint32_t, uint64_t, false> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t to, len;
        cin >> to >> len;
        G.add_edge(i, to - 1, len);
    }
    G.prepare();

    std::vector<uint64_t> ans(G.cycle_count());
    std::vector<uint64_t> out_ray(n);
    // 先在环上的每个点，往环外搜
    for (uint32_t i = 0; i != n; i++)
        if (!G.get_vertex_info(i).step_to_cycle()) {
            uint32_t cycle_id = G.get_vertex_info(i).cycle_id();
            auto dfs = [&](auto self, uint32_t a) -> uint64_t {
                uint64_t res = 0;
                G.do_for_out_edges(a, [&](uint32_t to, uint32_t dis) {
                    uint64_t ray = self(self, to);
                    ans[cycle_id] = std::max(ans[cycle_id], res + ray + dis);
                    res = std::max(res, ray + dis);
                });
                return res;
            };
            out_ray[i] = dfs(dfs, i);
        }
    // 在环上 dp
    for (uint32_t id = 0; id != G.cycle_count(); id++) {
        auto cycle = G.get_cycle_info(id);
        // 统计从 l 正向连到 r 的最长链
        uint64_t pre = 0;
        for (uint32_t r = 0; r != cycle.size(); r++) {
            if (r) pre += cycle.position(r) - cycle.position(r - 1);
            uint32_t x = cycle[r];
            ans[id] = std::max(ans[id], pre + out_ray[x]);
            pre = std::max(pre, out_ray[x]);
        }
        // 统计从 l 反向连到 r 的最长链
        pre = 0;
        for (uint32_t r = 0; r != cycle.size(); r++) {
            uint32_t x = cycle[r];
            if (r) ans[id] = std::max(ans[id], pre + out_ray[x] + (cycle.length() - cycle.position(r)));
            pre = std::max(pre, out_ray[x] + cycle.position(r));
        }
    }
    uint64_t sum = 0;
    for (uint64_t a : ans) sum += a;
    cout << sum;
}