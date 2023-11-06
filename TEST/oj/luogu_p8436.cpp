#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[P8436 【模板】边双连通分量](https://www.luogu.com.cn/problem/P8436)
*/
/**
 * 本题为边双连通分量模板题
 */

static constexpr uint32_t N = 500000, M = 2000000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::EBCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    auto ebccs = G.get_ebccs();
    cout << ebccs.size() << endl;

    for (auto &ebcc : ebccs) {
        cout << ebcc.size();
        for (auto a : ebcc) cout << ' ' << a + 1;
        cout << '\n';
    }
}