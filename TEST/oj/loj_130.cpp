#include "DS/BIT.h"
#include "DS/GlobalHashBIT.h"
#include "DS/CompressedTree.h"
#include "DS/MonoZkwTree.h"
#include "DS/SegTree.h"
#include "DS/WTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
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
        int64_t x;
        cin >> x;
        return x;
    };
    OY::WTree::Tree<int64_t> S(n, read);
    // auto S = [&]() {
    //     OY::GHashBIT<uint32_t, int64_t, false, false, 1500007> S(n);
    //     for (uint32_t i = 0; i != n; i++) {
    //         int64_t x;
    //         cin >> x;
    //         S.add(i, x);
    //     }
    //     return S;
    // }();
    // OY::StaticBIT<int64_t, false, 1 << 20> S(n, read);
    // OY::StaticCompressedSumTree<int64_t, 0, uint32_t, 1 << 21> S(n, read);
    // OY::MonoSumTree<int64_t> S(n, read);
    // OY::ZkwSumTree<int64_t> S(n, read);
    // OY::StaticSegSumTree<int64_t, true, uint32_t, 1 << 21> S(n, read);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            int64_t k;
            cin >> x >> k;
            S.add(x - 1, k);
            // S.modify(x - 1, S.query(x - 1) + k);
        } else {
            uint32_t x, y;
            cin >> x >> y;
            cout << S.query(x - 1, y - 1) << endl;
        }
    }
}