#include "DS/BIT.h"
#include "DS/GlobalHashBIT.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[P3368 【模板】树状数组 2](https://www.luogu.com.cn/problem/P3368)
*/
/**
 * 本题要进行区间增加和单点查询
 * 可以转化为差分值的单点增加和区间查询
 * 为树状数组模板题
 * 然而，目前最快的代码应当是用 GNU 扩展所打造的 WTREE 模板
 */

int32_t last;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::WTree::Tree<int> S(n, [](auto...) {
        int x, y;
        cin >> x;
        y = x - last, last = x;
        return y;
    });
    // OY::BIT32<false, 1 << 19> S(n, [](auto...) {
    //     int x, y;
    //     cin >> x;
    //     y = x - last, last = x;
    //     return y;
    // });
    // auto S = [&]() {
    //     OY::GHashBIT<uint32_t, int, false, false, 20> S(n);
    //     for (uint32_t i = 0; i != n; i++) {
    //         int64_t x = last;
    //         cin >> last;
    //         S.add(i, last - x);
    //     }
    //     return S;
    // }();
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y;
            int k;
            cin >> x >> y >> k;
            S.add(x - 1, k);
            if (y != n) S.add(y, -k);
        } else {
            uint32_t x;
            cin >> x;
            cout << S.presum(x - 1) << endl;
        }
    }
}