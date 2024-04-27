#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[Range Affine Point Get](https://judge.yosupo.jp/problem/range_affine_point_get)(https://github.com/yosupo06/library-checker-problems/issues/778)
*/
/**
 * 本题为区间更新线段树模板题
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
    using value_type = node;
    using modify_type = node;
    static constexpr bool init_clear_lazy = true;
    modify_type m_val;
    static void map(const modify_type &inc, Node *x, uint32_t) {}
    static void map(const modify_type &inc, Node *x) { x->m_val = x->m_val + inc; }
    static void com(const modify_type &inc, Node *x) { x->m_val = x->m_val + inc; }
    void pushup(Node *, Node *) {}
    const value_type &get() const { return m_val; }
    void set(const value_type &val) {}
    void init_set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_val.mul != 1 || m_val.add; }
    const modify_type &get_lazy() const { return m_val; }
    void clear_lazy() { m_val = {1, 0}; }
};
using Zkw = OY::ZKW::Tree<Node>;

int main() {
    uint32_t n, q;
    cin >> n >> q;
    std::vector<uint32_t> arr(n);
    std::for_each(arr.begin(), arr.end(), [&](auto &&e) { cin >> e; });

    Zkw S(n, [](auto...) {
        return node{1, 0};
    });
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r, b, c;
            cin >> l >> r >> b >> c;
            S.add(l, r - 1, {b, c});
        } else {
            uint32_t i;
            cin >> i;
            cout << S.query(i).calc(arr[i]) << endl;
        }
    }
}
