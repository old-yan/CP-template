#include "DS/AssignZkwTree.h"
#include "IO/FastIO.h"

/*
[Range Set Range Composite](https://judge.yosupo.jp/problem/range_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/829)
*/
/**
 * 本题为区间更新区间查询线段树模板题
 * 注意当使用普通 Zkw 时，复杂度为两个 log
 * 在这种需要类似快速幂的方式去计算一个区间的聚合值时，使用 AssignZkw 效率更高，复杂度为一个 log
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
    auto S = OY::make_lazy_AssignZkwTree<node>(n, [&](auto...) {
        uint32_t mul, add;
        cin >> mul >> add;
        return node{mul, add};
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r, mul, add;
            cin >> l >> r >> mul >> add;
            S.modify(l, r - 1, {mul, add});
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            cout << S.query(l, r - 1).calc(x) << endl;
        }
    }
}
