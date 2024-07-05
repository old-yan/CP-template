#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[U238155 【模板】割边](https://www.luogu.com.cn/problem/U238155)
*/
/**
 * 本题为桥边模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::EBCC::Graph G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    auto sol = G.calc<true, false>();
    cout << sol.m_bridge_cnt << endl;

    auto print = [&](uint32_t index) {
        auto [from, to] = G.m_raw_edges[index];
        cout << from + 1 << ' ' << to + 1 << endl;
    };
    sol.do_for_each_bridge(print);
}