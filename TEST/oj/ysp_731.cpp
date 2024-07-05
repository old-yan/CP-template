#include "GRAPH/Tarjan_cut.h"
#include "IO/FastIO.h"

/*
[Biconnected Components](https://judge.yosupo.jp/problem/biconnected_components)(https://github.com/yosupo06/library-checker-problems/issues/731)
*/
/**
 * 本题为 tarjan 算法求 ebcc 模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::VBCC::Graph G(n, m);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a, b);
    }
    auto sol = G.calc<false, true>();

    cout << sol.m_vbcc_cnt << endl;
    sol.do_for_each_vbcc([&](auto first, auto last) {
        cout << last - first;
        for (auto it = first; it != last; ++it) cout << ' ' << *it;
        cout << endl;
    });
}