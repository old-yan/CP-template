#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P3373 【模板】线段树 2](https://www.luogu.com.cn/problem/P3373)
*/
/**
 * 本题要进行区间修改和区间查询
 * 为线段树模板题
 */

using mint = OY::StaticMontgomeryModInt32<571373, true>;
// using mint = OY::StaticModInt32<571373, true>;
struct node {
    mint mul, add;
};
auto map = [](node modify, mint old_val, uint32_t len) { return old_val * modify.mul + modify.add * len; };
auto com = [](node new_modify, node old_modify) { return node{old_modify.mul * new_modify.mul, old_modify.add * new_modify.mul + new_modify.add}; };

int main() {
    uint32_t n, m, p;
    cin >> n >> m >> p;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto tree = OY::make_lazy_ZkwTree<mint, node, true, 100000 << 2>(n, read, std::plus<>(), map, com, node{1, 0});
    // auto tree = OY::make_lazy_SegTree<mint, node, true, true, OY::Seg::Ignore, 100000 << 1>(n, read, Op(), Map(), Com(), node{1, 0});
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            tree.add(l - 1, r - 1, {x, 0});
        } else if (op == '2') {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            tree.add(l - 1, r - 1, {1, x});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << tree.query(l - 1, r - 1) << '\n';
        }
    }
}