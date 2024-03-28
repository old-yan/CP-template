#include "DS/BIT.h"
#include "DS/GlobalHashBIT.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[P3374 【模板】树状数组 1](https://www.luogu.com.cn/problem/P3374)
*/
/**
 * 本题要进行单点修改和区间查询
 * 为树状数组模板题
 * 然而，目前最快的代码应当是用 GNU 扩展所打造的 WTREE 模板
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::WTree::Tree<int> S(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    // OY::BIT32<false, 1 << 19> S(n, [](auto...) {
    //     int x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = [&]() {
    //     OY::GHashBIT<uint32_t, int, false, false, 20> S(n);
    //     for (uint32_t i = 0; i != n; i++) {
    //         int x;
    //         cin >> x;
    //         S.add(i, x);
    //     }
    //     return S;
    // }();
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            int k;
            cin >> x >> k;
            S.add(x - 1, k);
        } else {
            uint32_t x, y;
            cin >> x >> y;
            cout << S.query(x - 1, y - 1) << endl;
        }
    }
}
