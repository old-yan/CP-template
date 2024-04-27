#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[Range Set Range Composite](https://judge.yosupo.jp/problem/range_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/829)
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
    node operator*(uint32_t len) const {
        node res{1, 0}, a{*this};
        while (len) {
            if (len & 1) res = res + a;
            a = a + a, len >>= 1;
        }
        return res;
    }
};
struct Node {
    using value_type = node;
    using modify_type = node;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(const modify_type &modify, Node *x, uint32_t len) {
        x->m_val = modify * len;
    }
    static void map(const modify_type &modify, Node *x) {
        x->m_val = modify;
    }
    static void com(const modify_type &modify, Node *x) {
        x->m_inc = modify;
    }
    value_type m_val;
    modify_type m_inc;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_inc.mul; }
    const modify_type &get_lazy() const { return m_inc; }
    void clear_lazy() { m_inc = {}; }
};

using Zkw = OY::ZKW::Tree<Node>;

int main() {
    uint32_t n, q;
    cin >> n >> q;
    Zkw S(n, [&](auto...) {
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
            S.add(l, r - 1, {mul, add});
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            cout << S.query(l, r - 1).calc(x) << endl;
        }
    }
}