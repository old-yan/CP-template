#include "DS/BIT.h"
#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "DS/GlobalHashBIT.h"
#include "IO/FastIO.h"

/*
[P3372 【模板】线段树 1](https://www.luogu.com.cn/problem/P3372)
*/
/**
 * 本题要进行区间修改和区间查询
 * 为线段树模板题
 * 也可以使用树状数组解决，而且更快
 */

static constexpr uint32_t N = 100000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BIT64<true, N * 2> tree(n, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    // OY::ZkwLazySumTree<N << 2> tree(n, [](auto...) {
    //     int64_t x;
    //     cin >> x;
    //     return x;
    // });
    // OY::SegLazySumTree<true, uint32_t, N << 1> tree(n, [](auto...) {
    //     int64_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto tree = [&]() {
    //     OY::GHashBIT<uint32_t, int64_t, true, false, 19> tree(n);
    //     for (uint32_t i = 0; i != n; i++) {
    //         int64_t x;
    //         cin >> x;
    //         tree.add(i, x);
    //     }
    //     return tree;
    // }();
    for (uint32_t i = 0; i < m; i++) {
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
            cout << tree.query(l - 1, r - 1) << '\n';
        }
    }
}