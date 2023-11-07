#include "GRAPH/Tarjan_cut.h"
#include "IO/FastIO.h"

/*
[P3388 【模板】割点（割顶）](https://www.luogu.com.cn/problem/P3388)
*/
/**
 * 本题为割点模板题
 */

static constexpr uint32_t N = 20000, M = 100000;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::VBCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    auto sol = G.calc<true, false>();
    cout << sol.m_cut_cnt << endl;
    sol.do_for_each_cut([](uint32_t i) {
        cout << i + 1 << ' ';
    });
}