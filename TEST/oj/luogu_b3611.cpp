#include "GRAPH/FloydWarshall.h"
#include "IO/FastIO.h"

/*
[B3611 【模板】传递闭包](https://www.luogu.com.cn/problem/B3611)
*/
/**
 * 本题为传递闭包模板题，需要用到 FloydWayshall 算法
 */
static constexpr uint32_t N = 100;
int main() {
    uint32_t n;
    cin >> n;
    OY::FloydWarshall::Graph<N> G(n);
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            char c;
            cin >> c;
            if (c == '1') G.add_edge(i, j);
        }
    }

    G.calc();
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            cout << bool(G.m_can[i][j]) << " \n"[j == n - 1];
        }
    }
}
