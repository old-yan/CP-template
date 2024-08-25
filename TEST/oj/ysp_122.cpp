#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

/*
[Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/122)
*/
/**
 * 本题为普通线段树模板题
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
    bool operator!=(const node &rhs) const { return mul != rhs.mul || add != rhs.add; }
};

constexpr node identity{1, 0};
int main() {
    uint32_t n, q;
    cin >> n >> q;
    auto S = OY::MonoSumTree<node, identity>(n, [](auto...) {
        uint32_t a, b;
        cin >> a >> b;
        return node{a, b};
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t p, c, d;
            cin >> p >> c >> d;
            S.modify(p, {c, d});
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            cout << S.query(l, r - 1).calc(x) << endl;
        }
    }
}
