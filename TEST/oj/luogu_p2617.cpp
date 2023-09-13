#include "DS/SegBIT.h"
#include "IO/FastIO.h"

/*
[P2617 Dynamic Rankings](https://www.luogu.com.cn/problem/P2617)
*/
using Tree = OY::SegBITSumTree<false, uint32_t, 1 << 20, 30000000>;

uint32_t A[100001];
static constexpr uint32_t M = 1000000000;
int main() {
    sizeof(Tree::node);
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 1; i <= n; i++) cin >> A[i];
    Tree S(n + 1, M + 1);

    for (uint32_t i = 1; i <= n; i++) S.add(i, A[i], 1);
    for (uint32_t i = 0; i < m; i++) {
        char c;
        cin >> c;
        if (c == 'Q') {
            uint32_t l, r, k;
            cin >> l >> r >> k;
            cout << S.kth(l, r, k - 1) << '\n';
        } else {
            uint32_t pos, k;
            cin >> pos >> k;
            S.add(pos, A[pos], -1);
            S.add(pos, A[pos] = k, 1);
        }
    }
}