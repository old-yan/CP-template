#include "GRAPH/Hungarian.h"
#include "GRAPH/HopcroftKarp.h"
#include "IO/FastIO.h"

/*
[#78. 二分图最大匹配](https://uoj.ac/problem/78)
*/
/**
 * 本题为二分图最大匹配模板题
 * 需要输出方案
 */
static constexpr uint32_t N = 500, M = 250000;
int main() {
    uint32_t n1, n2, m;
    cin >> n1 >> n2 >> m;
    OY::HK::Graph<N, M> G(std::max(n1, n2), m);
    // OY::HG::Graph<N, M> G(std::max(n1, n2), m);
    for (uint32_t i = 0; i < m; i++) {
        uint32_t a, b;
        cin >> a >> b;
        G.add_edge(a - 1, b - 1);
    }

    cout << G.calc() << endl;
    for (uint32_t i = 0; i < n1; i++) cout << G.find_right(i) + 1 << ' ';
}
