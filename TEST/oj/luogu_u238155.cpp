#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[U238155 【模板】割边](https://www.luogu.com.cn/problem/U238155)
*/
/**
 * 本题为桥边模板题
 */

static constexpr uint32_t N = 20000, M = 100000;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::EBCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    auto sol = G.calc<true, false>();
    cout << sol.m_bridge_cnt << endl;

    auto print = [&](uint32_t index) {
        uint32_t from = G.m_edges[index].m_from, to = G.m_edges[index].m_to;
        cout << from + 1 << ' ' << to + 1 << endl;
    };
    sol.do_for_each_bridge(print);
}