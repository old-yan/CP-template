#include "GRAPH/Tarjan_cut.h"
#include "IO/FastIO.h"

/*
[T225524 【模板】点双连通分量（v-dcc）](https://www.luogu.com.cn/problem/T225524)
*/
/**
 * 本题为点双连通分量模板题
 */

static constexpr uint32_t N = 200000, M = 200000;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::VBCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    auto sol = G.calc<false, true>();
    cout << sol.m_vbcc_cnt << endl;
    sol.do_for_each_vbcc([](uint32_t *first, uint32_t *last) {
        cout << last - first;
        for (auto it = first; it != last; ++it) cout << ' ' << *it + 1;
        cout << endl;
    });
}