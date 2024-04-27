#include "DS/SortSegTree.h"
#include "IO/FastIO.h"

/*
[Point Set Range Sort Range Composite](https://judge.yosupo.jp/problem/point_set_range_sort_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/818)
*/
/**
 * 本题为区间排序线段树模板题
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
int main() {
    uint32_t n, q;
    cin >> n >> q;
    std::vector<uint32_t> keys(n);
    std::vector<node> items(n);
    for (uint32_t i = 0; i < n; i++) cin >> keys[i] >> items[i].mul >> items[i].add;
    auto key_mapping = [&](uint32_t i) { return keys[i]; };
    auto mapping = [&](uint32_t i) { return items[i]; };
    using Tree = OY::SortSeg::Tree<uint32_t, node, OY::SortSeg::MAINTAIN_RANGE_REVERSE, 1 << 22>;
    Tree S(n, key_mapping, mapping, 1000000000, {1, 0});
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t i, p, a, b;
            cin >> i >> p >> a >> b;
            S.modify(i, p, {a, b});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            if (op == '1') {
                uint32_t x;
                cin >> x;
                cout << S.query(l, r - 1).calc(x) << endl;
            } else if (op == '2')
                S.sort<false>(l, r - 1);
            else
                S.sort<true>(l, r - 1);
        }
    }
}