#include "DS/LazyBitset.h"
#include "DS/SegTree.h"
#include "IO/FastIO.h"

/*
[小红的01串（六）](https://ac.nowcoder.com/acm/problem/285892)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/98256/F
 * 如果将奇数位置和偶数位置独立处理，可以直接套用 LazyBitset 模板
 */

void solve_bitset() {
    uint32_t n, q;
    cin >> n >> q;
    OY::VectorLazyBitset<uint32_t> S[2];
    S[0].resize(n + 1);
    S[1].resize(n + 1);
    for (uint32_t i = 0; i != q; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            if (r > l || !(l & 1)) S[0].set(l + 1 >> 1, r >> 1);
            if (r > l || (l & 1)) S[1].set(l >> 1, r - 1 >> 1);
        } else if (op == '2') {
            if (r > l || !(l & 1)) S[0].flip(l + 1 >> 1, r >> 1);
            if (r > l || (l & 1)) S[1].flip(l >> 1, r - 1 >> 1);
        } else {
            uint32_t cnt = 0;
            if (r > l || !(l & 1)) cnt += ((r >> 1) - (l + 1 >> 1) + 1) - S[0].count(l + 1 >> 1, r >> 1);
            if (r > l || (l & 1)) cnt += S[1].count(l >> 1, r - 1 >> 1);
            cout << std::min(cnt, r - l + 1 - cnt) << endl;
        }
    }
}

// 如果用一颗树进行处理也是可以的，但是树中结点必须知道自己管理的区间边界的奇偶性，还有区间长度
struct seg {
    // good 属性表示 [m_l, m_r] 区间内，值%2==下标%2 的位置总数
    uint32_t m_l, m_r, m_good;
    uint32_t len() { return m_r - m_l + 1; }
    void set_zero() { m_good = (m_r + 2) / 2 - (m_l + 1) / 2; }
    void set_one() { set_zero(), flip(); }
    void flip() { m_good = len() - m_good; }
    seg operator+(const seg &rhs) const { return {m_l, rhs.m_r, m_good + rhs.m_good}; }
};
struct Node {
    using value_type = seg;
    // 3 表示 set 修改,  2 表示 flip 修改， 0 表示 reset 修改
    using modify_type = uint32_t;
    using size_type = uint32_t;
    using node_type = Node;
    static constexpr bool init_clear_lazy = true;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(const modify_type &modify, node_type *x, size_type len) {
        if (modify == 3)
            x->m_val.set_one();
        else if (modify == 0)
            x->m_val.set_zero();
        else if (modify == 2)
            x->m_val.flip();
    }
    static void com(const modify_type &modify, node_type *x) {
        if (modify == 0 || modify == 3)
            x->m_modify = modify;
        else if (modify == 2)
            x->m_modify = 3 - x->m_modify;
    }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(char c) {
        if (c == '1')
            m_val.set_one();
        else
            m_val.set_zero();
    }
    void set(const value_type &val) { m_val = val; }
    bool has_lazy() const { return m_modify != 1; }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() { m_modify = 1; }
};
struct Mapping {
    seg operator()(uint32_t l, uint32_t r) const {
        seg x{l, r};
        x.set_zero();
        return x;
    }
};
void solve_seg() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Seg::Tree<Node, Mapping, uint32_t> S(n + 1);
    while (m--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1')
            S.add(l, r, 3);
        else if (op == '2')
            S.add(l, r, 2);
        else {
            auto res = S.query(l, r);
            cout << std::min(res.m_good, res.len() - res.m_good) << endl;
        }
    }
}

int main() {
    solve_bitset();
    // solve_seg();
}