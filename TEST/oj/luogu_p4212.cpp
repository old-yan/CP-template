#include "GRAPH/BronKerbosch.h"
#include "IO/FastIO.h"

/*
[P4212 外太空旅行](https://www.luogu.com.cn/problem/P4212)
*/
/**
 * 本题为 BronKerbosch 模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::BK::Graph<uint64_t, 50> G(n);
    uint32_t a, b;
    while (cin >> a >> b) G.add_edge(a - 1, b - 1);
    cout << G.max_clique();
}
