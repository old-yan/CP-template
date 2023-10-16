#include "DS/SegBIT.h"
#include "DS/SegTree2D.h"
#include "IO/FastIO.h"

/*
[P3380 【模板】二逼平衡树（树套树）](https://www.luogu.com.cn/problem/P3380)
*/
/**
 * 本题类似 P2617
 * 不妨在一个二维 01 数组上想象
 * 以下标为行号，以值为列号，在二维数组里填充 1
 * 区间排名，即为选定若干连续的行，查询某列左边有几个 1
 * 区间第 k 小值，即为选定若干行，在这些行里从左往右找第 k 个 1
 * 修改某点点值，即将某个 1 不改变行号，改变列号
 * 所以本题可以用二维线段树解决
 */
using Tree = OY::SegBITSumTree<false, uint32_t, 1 << 17, 40000000>;
// using Tree = OY::SegSumTree2D<false, uint32_t, 1 << 17, 40000000>;

uint32_t A[50001];
static constexpr uint32_t M = 100000000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 1; i <= n; i++) cin >> A[i];
    Tree S(n + 1, M + 1);

    for (uint32_t i = 1; i <= n; i++) S.add(i, A[i], 1);
    for (uint32_t i = 0; i < m; i++) {
        char c;
        cin >> c;
        if (c == '1') {
            uint32_t l, r, k;
            cin >> l >> r >> k;
            cout << (k ? S.query(l, r, 0, k - 1) + 1 : 1) << '\n';
        } else if (c == '2') {
            uint32_t l, r, k;
            cin >> l >> r >> k;
            cout << S.kth(l, r, k - 1) << '\n';
        } else if (c == '3') {
            uint32_t pos, k;
            cin >> pos >> k;
            S.add(pos, A[pos], -1);
            S.add(pos, A[pos] = k, 1);
        } else if (c == '4') {
            uint32_t l, r, k;
            cin >> l >> r >> k;
            auto v = k ? S.query(l, r, 0, k - 1) : 0;
            if (!v)
                cout << "-2147483647\n";
            else
                cout << S.kth(l, r, v - 1) << '\n';
        } else {
            uint32_t l, r, k;
            cin >> l >> r >> k;
            auto v = k < M ? S.query(l, r, k + 1, M) : 0;
            if (!v)
                cout << "2147483647\n";
            else
                cout << S.kth(l, r, S.query(l, r, 0, k)) << '\n';
        }
    }
}