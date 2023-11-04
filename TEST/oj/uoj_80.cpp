#include "GRAPH/KuhnMunkres.h"
#include "IO/FastIO.h"

/*
[#80. 二分图最大权匹配](https://uoj.ac/problem/80)
*/
/**
 * 本题为二分图最大权匹配模板题，需要输出方案
 */
static constexpr uint32_t N = 400, M = N * N;
int main() {
    uint32_t nl, nr, m;
    cin >> nl >> nr >> m;
    // 边权初始值设为零，这样就不会取负边
    OY::KM::Graph<int64_t, N, M> G(std::max(nl, nr), 0);

    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b, cost;
        cin >> a >> b >> cost;
        G.add_edge(a - 1, b - 1, cost);
    }

    cout << G.calc() << endl;

    // 可以观察最后的边权
    // 如果是正的，说明是配对成功
    // 如果是零，说明取的是默认边，没有配对
    for (uint32_t l = 0; l < nl; l++) {
        uint32_t r = G.find_right(l);
        if (G.query(l, r))
            cout << r + 1 << ' ';
        else
            cout << 0 << ' ';
    }
}