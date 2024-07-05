#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[P8436 【模板】边双连通分量](https://www.luogu.com.cn/problem/P8436)
*/
/**
 * 本题为边双连通分量模板题
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

    auto sol = G.calc<false, true>();
    cout << sol.m_ebcc_cnt << endl;

    sol.do_for_each_ebcc([](uint32_t *first, uint32_t *last) {
        cout << last - first;
        for (auto it = first; it != last; ++it) cout << ' ' << *it + 1;
        cout << '\n';
    });
}