#include "DS/LichaoSegTree.h"
#include "DS/LichaoSegTree_ex.h"
#include "DS/LichaoZkwTree.h"
#include "DS/LichaoZkwTree_ex.h"
#include "IO/FastIO.h"

#include <climits>

/*
[智乃的直线](https://ac.nowcoder.com/acm/problem/226924)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/19917/A
 * 本题使用单点修改线段树可以解决，每个叶子节点处有一个堆，堆的最大值作为叶子处的值
 */

void solve_lc() {
    uint32_t n, m;
    cin >> n >> m;
    struct Line {
        int m_k, m_b;
        Line() = default;
        Line(int k, int b) : m_k(k), m_b(b) {}
        int64_t calc(int i) const { return int64_t(m_k) * i + m_b; }
    };
    OY::LCSEG::Tree<Line, std::less<>> S_max(n + 1, {-10000, INT_MIN});
    // OY::LCZKW::Tree<Line, std::less<>> S_max(n + 1, {-10000, INT_MIN});
    OY::LCSEG::Tree<Line, std::greater<>> S_min(n + 1, {10000, INT_MAX});
    // OY::LCZKW::Tree<Line, std::greater<>> S_min(n + 1, {10000, INT_MAX});
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            int k, b;
            cin >> k >> b;
            S_max.add(0, n, {k, b});
            S_min.add(0, n, {k, b});
        } else {
            uint32_t x;
            cin >> x;
            cout << S_max.query(x).calc(x) << ' ' << S_min.query(x).calc(x) << endl;
        }
    }
}

void solve_lcex() {
    uint32_t n, m;
    cin >> n >> m;
    OY::VectorLichaoSlopeChmaxSegTree_ex<int64_t, INT64_MIN, false, OY::LCSEGEX::BaseLine<int64_t>, uint32_t> S_max(n + 1);
    // OY::LichaoSlopeChmaxZkwTree_ex<int64_t, INT64_MIN, false, OY::LCZKWEX::BaseLine<int64_t>> S_max(n + 1);
    OY::VectorLichaoSlopeChminSegTree_ex<int64_t, INT64_MAX, false, OY::LCSEGEX::BaseLine<int64_t>, uint32_t> S_min(n + 1);
    // OY::LichaoSlopeChminZkwTree_ex<int64_t, INT64_MAX, false, OY::LCZKWEX::BaseLine<int64_t>> S_min(n + 1);
    while (m--) {
        char op;
        cin >> op;
        if (op == '0') {
            int k, b;
            cin >> k >> b;
            S_max.add_line(0, n, {k, b});
            S_min.add_line(0, n, {k, b});
        } else {
            uint32_t x;
            cin >> x;
            cout << S_max.query(x) << ' ' << S_min.query(x) << endl;
        }
    }
}

int main() {
    solve_lc();
    // solve_lcex();
}