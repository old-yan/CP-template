#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

/*
[P4513 小白逛公园](https://www.luogu.com.cn/problem/P4513)
*/
/**
 * 本题为最大子段和经典题
 * 或者说最大子数组和
 * 两个子段合并的最值，要么是两个子段的最值之一，要么是前一段的后缀最大加上后一段的前缀最大
 */

void solve_zkw() {
    using Tp = int;
    static constexpr Tp inf = 0x3f3f3f3f;
    struct monoid {
        struct value_type {
            Tp m_lmx, m_rmx, m_mx, m_sum;
            value_type() = default;
            value_type(Tp x) : m_lmx(x), m_rmx(x), m_mx(x), m_sum(x) {}
            value_type(Tp lmx, Tp rmx, Tp mx, Tp sum) : m_lmx(lmx), m_rmx(rmx), m_mx(mx), m_sum(sum) {}
            value_type operator+(const value_type &rhs) const {
                return value_type(std::max(m_lmx, m_sum + rhs.m_lmx), std::max(m_rmx + rhs.m_sum, rhs.m_rmx), std::max({m_mx, rhs.m_mx, m_rmx + rhs.m_lmx}), m_sum + rhs.m_sum);
            }
        };
        static value_type identity() { return {-inf, -inf, -inf, 0}; }
        static value_type op(const value_type &x, const value_type &y) { return x + y; }
    };
    uint32_t n, m;
    cin >> n >> m;
    auto read = [](auto...) {
        int x;
        cin >> x;
        return x;
    };
    OY::MONOZKW::Tree<monoid> S(n, read);
    while (m--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r;
            if (l > r) std::swap(l, r);
            cout << S.query(l - 1, r - 1).m_mx << endl;
        } else {
            uint32_t pos;
            int val;
            cin >> pos >> val;
            S.modify(pos - 1, val);
        }
    }
}

int main() {
    solve_zkw();
}