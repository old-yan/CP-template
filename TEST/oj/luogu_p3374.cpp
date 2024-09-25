#include "DS/BIT.h"
#include "DS/MonoBIT.h"
#include "DS/MonoZkwTree.h"
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
    auto read = [](auto...) {
        int x;
        cin >> x;
        return x;
    };
    OY::WSumTree<int> S(n, read);
    // OY::StaticBIT<int, false, 1 << 19> S(n, read);
    // OY::MonoSumBIT<int> S(n, read);
    // OY::MonoSumTree<int> S(n, read);
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
