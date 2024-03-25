#include "DS/BIT.h"
#include "DS/GlobalHashBIT.h"
#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[#132. 树状数组 3 ：区间修改，区间查询](https://loj.ac/p/132)
*/
/**
 * 本题为二维树状数组模板题
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::BIT64<true, 1 << 20> tree(n, [](auto...) -> int64_t {
        int64_t x;
        cin >> x;
        return x;
    });
    // OY::ZkwLazySumTree<1 << 21> tree(n, [](auto...) -> int64_t {
    //     int64_t x;
    //     cin >> x;
    //     return x;
    // });
    // OY::SegLazySumTree<true, uint32_t, 1 << 21> tree(n, [](auto...) -> int64_t {
    //     int64_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto tree = [&]() {
    //     OY::GHashBIT<uint32_t, int64_t, true, false, 21> tree(n);
    //     for (uint32_t i = 0; i < n; i++) {
    //         int64_t x;
    //         cin >> x;
    //         tree.add(i, x);
    //     }
    //     return tree;
    // }();
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            int64_t x;
            cin >> l >> r >> x;
            tree.add(l - 1, r - 1, x);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << tree.query(l - 1, r - 1) << endl;
        }
    }
}