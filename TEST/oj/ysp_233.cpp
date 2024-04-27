#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/233)
*/
/**
 * 本题为区间更新区间查询线段树模板题
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
struct Node {
    using value_type = uint32_t;
    using modify_type = node;
    static constexpr bool init_clear_lazy = true;
    static value_type op(value_type x, value_type y) {
        x += y;
        if (x >= P) x -= P;
        return x;
    }
    static void map(const modify_type &modify, Node *x, uint32_t len) {
        x->m_val = ((uint64_t)x->m_val * modify.mul + (uint64_t)modify.add * len) % P;
    }
    static void com(const modify_type &modify, Node *x) {
        x->m_inc = x->m_inc + modify;
    }
    value_type m_val;
    modify_type m_inc;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_inc.mul != 1 || m_inc.add != 0; }
    const modify_type &get_lazy() const { return m_inc; }
    void clear_lazy() { m_inc = {1, 0}; }
};

using Zkw = OY::ZKW::Tree<Node>;

int main() {

    uint32_t n, q;
    cin >> n >> q;
    Zkw S(n, [&](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r, mul, add;
            cin >> l >> r >> mul >> add;
            S.add(l, r - 1, {mul, add});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query(l, r - 1) << endl;
        }
    }
}