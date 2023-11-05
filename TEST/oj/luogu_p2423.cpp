#include "GRAPH/BronKerbosch.h"
#include "IO/FastIO.h"

/*
[P2423 [HEOI2012] 朋友圈](https://www.luogu.com.cn/problem/P2423)
*/
/**
 * 本题正解为二分图最大团，但是可以用 BK 跑过去
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t A, B, M;
        cin >> A >> B >> M;

        OY::BK::Graph<uint64_t, 3010> G(A + B);
        std::vector<uint32_t> a(A), b(B);
        for (auto &e : a) cin >> e;
        for (uint32_t i = 0; i < A; i++)
            for (uint32_t j = i + 1; j < A; j++)
                if ((a[i] ^ a[j]) & 1) G.add_edge(i, j);

        for (auto &e : b) cin >> e;
        for (uint32_t i = 0; i < B; i++)
            for (uint32_t j = i + 1; j < B; j++)
                if (!((b[i] ^ b[j]) & 1) || std::popcount(b[i] | b[j]) % 2) G.add_edge(A + i, A + j);

        for (uint32_t i = 0; i < M; i++) {
            uint32_t x, y;
            cin >> x >> y;
            G.add_edge(x - 1, A + y - 1);
        }

        cout << G.max_clique() << endl;
    }
}
