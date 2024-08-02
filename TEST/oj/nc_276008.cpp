#include "GRAPH/Psuedotree_ig.h"
#include "IO/FastIO.h"

/*
[爱探险的朵拉](https://ac.nowcoder.com/acm/problem/276008)
*/
/**
 * 上面链接打不开的话，请访问https://ac.nowcoder.com/acm/contest/86639/B
 * 基环树模板题
 * 和 P2921 类似
 */

void solve_psue() {
    uint32_t n;
    cin >> n;
    OY::PsuedoIG::Graph<bool> G(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        G.set_edge(i, x - 1);
    }
    G.prepare();
    uint32_t ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        auto info = G.get_vertex_info(i);
        auto cycle = G.get_cycle_info(info.cycle_id());
        ans = std::max(ans, info.step_to_cycle() + cycle.size());
    }
    cout << ans;
}

int main() {
    solve_psue();
}