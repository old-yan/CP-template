#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[T103481 【模板】割边](https://www.luogu.com.cn/problem/T103481)
*/
/**
 * 本题为桥边模板题
 */

static constexpr uint32_t N = 50000, M = 300000;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::EBCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    cout << G.calc<true, false>().m_bridge_cnt;
}