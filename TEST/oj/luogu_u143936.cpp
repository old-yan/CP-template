#include "GRAPH/Tarjan_bridge.h"
#include "IO/FastIO.h"

/*
[U143936 割边/桥（模板）](https://www.luogu.com.cn/problem/U143936)
*/
/**
 * 本题为桥边模板题
 */

static constexpr uint32_t N = 300000, M = 1000000;
int main() {
    uint32_t n, m;
    cin >> n >> m;

    OY::EBCC::Graph<N, M> G(n, m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    uint32_t sum = 0;
    auto call = [&](uint32_t index) { sum ^= index + 1; };
    G.calc<true, false>().do_for_each_bridge(call);
    cout << sum;
}