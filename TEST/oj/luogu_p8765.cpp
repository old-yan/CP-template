#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P8765 [蓝桥杯 2021 国 AB] 翻转括号序列](https://www.luogu.com.cn/problem/P8765)
*/
/**
 * 按照套路，把左括号赋权 1，右括号赋权 -1
 * 维护前缀和数组 pre
 *
 * 区间修改，会使得 pre[l~r] 区间内的波形上下翻转，是一个线性变换；pre[r~] 区间后的波形是一个竖直方向的升降
 *
 * 给定 l ，求最大 r 使得 s[l~r] 合法。那么，pre[l-1]==pre[r]，且要求中间没有比 pre[l-1] 小的
 * 我们先找到 l 右侧第一个比 pre[l-1] 小的地方 end，r 只可能出现在这之前
 * 由于 pre 值是连续变化的，所以其实 end-1 就是答案
 */

struct presum {
    int m_min, m_max;
    presum operator+(const presum &rhs) const {
        return {std::min(m_min, rhs.m_min), std::max(m_max, rhs.m_max)};
    }
};
struct affine {
    uint32_t mul = 1, add;
    affine operator+(const affine &rhs) const {
        return {mul * rhs.mul, add * rhs.mul + rhs.add};
    }
};
struct Node {
    using value_type = presum;
    using modify_type = affine;
    using node_type = Node;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(modify_type modify, node_type *x, auto) {
        if (modify.mul == -1) std::swap(x->m_val.m_min, x->m_val.m_max), x->m_val.m_min = -x->m_val.m_min, x->m_val.m_max = -x->m_val.m_max;
        x->m_val.m_min += modify.add, x->m_val.m_max += modify.add;
    }
    static void com(modify_type modify, node_type *x) { x->m_modify = x->m_modify + modify; }
    value_type m_val;
    modify_type m_modify;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    const modify_type &get_lazy() const { return m_modify; }
    void clear_lazy() { m_modify = affine{}; }
};
void solve_zkw() {
    uint32_t n, m;
    std::string s;
    cin >> n >> m >> s;
    OY::ZKW::Tree<Node> S(n, [&, cur = 0](uint32_t i) mutable -> presum {
        cur += (s[i] == '(') ? 1 : -1;
        return {cur, cur};
    });
    auto print = [&] {
        for (int i = 0; i < n; i++) cout << S.query(i).m_min << " \n"[i == n - 1];
    };
    while (m--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r, l--, r--;
            uint32_t v0 = l ? S.query(l - 1).m_min : 0;
            uint32_t v1 = S.query(r).m_min;
            S.add(l, r, {0xffffffff, v0 * 2});
            if (r != n - 1) S.add(r + 1, n - 1, {1, v0 * 2 - v1 * 2});
        } else {
            uint32_t l;
            cin >> l, l--;
            auto v0 = l ? S.query(l - 1).m_min : 0;
            auto end = S.max_right(l, [v0](auto v) {
                return v.m_min >= v0;
            });
            if (end == l - 1 || end == l)
                cout << "0\n";
            else if (end != n - 1)
                cout << end + 1 << endl;
            else {
                auto r = S.min_left(n - 1, [v0](auto v) {
                    return v.m_min > v0;
                });
                if (r == l)
                    cout << "0\n";
                else
                    cout << r << endl;
            }
        }
    }
}

int main() {
    solve_zkw();
}