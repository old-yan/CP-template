#include "GRAPH/BFS.h"
#include "IO/FastIO.h"

/*
[P1443 马的遍历](https://www.luogu.com.cn/problem/P1443)
*/
/**
 * 本题为 BFS 算法求最短路模板题
 */
static constexpr uint32_t M = 400, N = 400;
static constexpr uint32_t dir[4][2] = {{-1u, 2}, {1, 2}, {2, 1}, {2, -1u}};
int main() {
    uint32_t m, n, r, c;
    cin >> m >> n >> r >> c;

    OY::BFS::Graph<M * N, M * N * 8> G(m * n, m * n * 8);
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < n; j++) {
            for (uint32_t k = 0; k < 4; k++) {
                uint32_t ii = i + dir[k][0], jj = j + dir[k][1];
                if (ii < m && jj < n) {
                    G.add_edge(i * n + j, ii * n + jj);
                    G.add_edge(ii * n + jj, i * n + j);
                }
            }
        }
    }

    auto res = G.calc<false>((r - 1) * n + (c - 1));

    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < n; j++) {
            auto dis = res.query(i * n + j);
            if (dis < m * n)
                cout << dis << ' ';
            else
                cout << "-1 ";
        }
        cout << endl;
    }
}