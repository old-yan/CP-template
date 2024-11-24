#include "DS/CompressedTree.h"
#include "IO/FastIO.h"

/*
[Point Set Range Composite (Large Array)](https://judge.yosupo.jp/problem/point_set_range_composite_large_array)(https://github.com/yosupo06/library-checker-problems/issues/828)
*/
/**
 * 本题为超大区间上，单点更新，区间查询线段树模板题
 */

static constexpr uint32_t P = 998244353;
struct node {
    uint32_t mul, add;
    uint32_t calc(uint64_t i) const {
        return (i * mul + add) % P;
    }
    node operator+(const node &rhs) const {
        return node{uint32_t((uint64_t)mul * rhs.mul % P), uint32_t(((uint64_t)add * rhs.mul + rhs.add) % P)};
    }
};

constexpr node identity{1, 0};
int main() {
    uint32_t n, q;
    cin >> n >> q;
    using Tree = OY::CPTREE::Tree<OY::CPTREE::BaseMonoid<node, identity, std::plus<>>, uint32_t>;
    Tree::_reserve(q * 2 + 100);
    Tree S;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t p, mul, add;
            cin >> p >> mul >> add;
            S.modify(p, {mul, add});
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            cout << S.query(l, r - 1).calc(x) << endl;
        }
    }
}
