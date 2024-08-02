#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"

/*
[P6037 Ryoku 的探索](https://www.luogu.com.cn/problem/P6037)
*/
/**
 * 本题为基环树模板题
 * 本题的要求，显然就是扔掉一条环边
 * 扔掉的环边必然是环上起点的左边或者右边
 */

struct EdgeCost {
    uint32_t len, p;
    friend uint32_t operator+(uint32_t x, const EdgeCost &) { return x + 1; }
    friend uint32_t operator+(const EdgeCost &, uint32_t y) { return y + 1; }
    friend uint32_t &operator+=(uint32_t &x, const EdgeCost &) { return ++x; }
};
int main() {
    uint32_t n;
    cin >> n;
    OY::PsuedoUG::Graph<EdgeCost, uint32_t, false> G(n);
    uint64_t total = 0;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t a, b, len, p;
        cin >> a >> b >> len >> p;
        G.add_edge(a - 1, b - 1, {len, p});
        total += len;
    }
    G.prepare();

    // 枚举边
    auto cycle = G.get_cycle_info(0);
    std::vector<EdgeCost> cycle_adj(cycle.size());
    std::vector<uint32_t> drop(cycle.size());
    for (auto &[from, to, e] : G.m_raw_edges)
        if (!G.get_vertex_info(from).step_to_cycle() && !G.get_vertex_info(to).step_to_cycle()) {
            uint32_t i1 = G.get_vertex_info(from).cycle_join();
            uint32_t i2 = G.get_vertex_info(to).cycle_join();
            if (!cycle_adj[i1].p)
                cycle_adj[i1] = e;
            else
                drop[i1] = e.p > cycle_adj[i1].p ? cycle_adj[i1].len : e.len;
            if (!cycle_adj[i2].p)
                cycle_adj[i2] = e;
            else
                drop[i2] = e.p > cycle_adj[i2].p ? cycle_adj[i2].len : e.len;
        }

    for (uint32_t i = 0; i != n; i++) {
        uint32_t join = G.get_vertex_info(i).cycle_join();
        cout << total - drop[join] << endl;
    }
}