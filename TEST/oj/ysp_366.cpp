#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[Two-Edge-Connected Components](https://judge.yosupo.jp/problem/two_edge_connected_components)(https://github.com/yosupo06/library-checker-problems/issues/366)
*/
/**
 * 本题为 tarjan 算法求 ebcc 模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::EBCC::Graph G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a, b);
    }
    auto sol = G.calc<false, true>();

    cout << sol.m_ebcc_cnt << endl;
    sol.do_for_each_ebcc([&](auto first, auto last) {
        cout << last - first;
        for (auto it = first; it != last; ++it) cout << ' ' << *it;
        cout << endl;
    });
}