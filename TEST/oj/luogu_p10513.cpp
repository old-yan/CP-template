#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P10513 括号](https://www.luogu.com.cn/problem/P10513)
*/
/**
 * 本题为最大匹配括号子序列经典题
 * 两个子段合并后的匹配括号数，分为三部分：
 *   第一个子段自己的匹配数
 *   第二个子段自己的匹配数
 *   第一个子段未匹配的左括号，和第二个子段未匹配的右括号
 * 
 * 加上一个区间 flip 修改后，需要同时维护 flip 前和 flip 后的答案，并作切换
 */

struct brackets {
    uint32_t m_l, m_r, m_sum0, m_sum1;
    brackets() = default;
    brackets(char c) : m_l(c == '('), m_r(c == ')'), m_sum0(0), m_sum1() {}
    brackets(uint32_t l, uint32_t r, uint32_t sum0, uint32_t sum1) : m_l(l), m_r(r), m_sum0(sum0), m_sum1(sum1) {}
    void flip() {
        std::swap(m_l, m_r);
        std::swap(m_sum0, m_sum1);
    }
    brackets operator+(const brackets &rhs) const {
        return {m_l + rhs.m_l, m_r + rhs.m_r, m_sum0 + rhs.m_sum0 + std::min(m_l - m_sum0, rhs.m_r - rhs.m_sum0), m_sum1 + rhs.m_sum1 + std::min(m_r - m_sum1, rhs.m_l - rhs.m_sum1)};
    }
};
struct Node {
    using value_type = brackets;
    using modify_type = bool;
    using node_type = Node;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(modify_type modify, node_type *x, auto) { x->m_val.flip(); }
    static void com(modify_type modify, node_type *x) { x->m_modify = !x->m_modify; }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    const modify_type &get_lazy() const { return m_modify; }
    bool has_lazy() const { return m_modify; }
    void clear_lazy() { m_modify = false; }
};
void solve_zkw() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    OY::ZKW::Tree<Node> S(s.begin(), s.end());
    uint32_t m;
    cin >> m;
    while (m--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1')
            S.add(l - 1, r - 1, true);
        else
            cout << S.query(l - 1, r - 1).m_sum0 << endl;
    }
}

int main() {
    solve_zkw();
}