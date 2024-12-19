#include "DS/ChthollyZkwTree.h"
#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

/*
[P5251 [LnOI2019] 第二代图灵机](https://www.luogu.com.cn/problem/P5251)
*/
/**
 * 考虑到数据随机，那么每次查询的区间 [l,r] 内的颜色段数量不会多
 * 找出这些颜色段，然后进行暴力计算
 */

struct monoid {
    struct value_type {
        uint32_t m_sum, m_min, m_max;
    };
    static value_type identity() { return {0, 0x3f3f3f3f, 0}; }
    static value_type op(const value_type &x, const value_type &y) { return {x.m_sum + y.m_sum, std::min(x.m_min, y.m_min), std::max(x.m_max, y.m_max)}; }
};
using SumMinMaxTree = OY::MONOZKW::Tree<monoid>;
void solve_czkw() {
    uint32_t n, m, c;
    cin >> n >> m >> c;

    SumMinMaxTree S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return monoid::value_type{x, x, x};
    });

    OY::ChthollyZkwTree<uint8_t> Color(n, 0);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t color;
        cin >> color;
        Color.modify(i, color);
    }
    for (uint32_t i = 0; i != m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x, y;
            cin >> x >> y, x--;
            S.modify(x, {y, y, y});
        } else if (op == '2') {
            uint32_t l, r, y;
            cin >> l >> r >> y, l--, r--;
            Color.modify(l, r, y);
        } else if (op == '3') {
            uint32_t l, r;
            cin >> l >> r, l--, r--;
            // 特判颜色数为 1
            if (c == 1)
                cout << S.query(l, r).m_min << endl;
            else {
                uint32_t cnt[101]{}, non_zero = 0, ans = -1;
                auto ranges = Color.get_ranges(l, r);
                for (uint32_t l = 0, r = 0; l != ranges.size();) {
                    while (r != ranges.size() && non_zero != c) non_zero += !cnt[ranges[r++].m_val]++;
                    if (non_zero == c) ans = std::min(ans, S.query(ranges[l].m_right, ranges[r - 1].m_left).m_sum);
                    non_zero -= !--cnt[ranges[l++].m_val];
                }
                cout << int(ans) << endl;
            }
        } else {
            uint32_t l, r;
            cin >> l >> r, l--, r--;
            __uint128_t mask = 0;
            uint32_t ans = 0;
            auto ranges = Color.get_ranges(l, r);
            for (uint32_t l = 0, r = 0; l != ranges.size();) {
                while (r != ranges.size()) {
                    if (mask >> (ranges[r].m_val) & 1) break;
                    if (r != l && ranges[r].m_left != ranges[r].m_right) break;
                    mask |= __uint128_t(1) << ranges[r++].m_val;
                }
                if (r != ranges.size() && !(mask >> ranges[r].m_val & 1))
                    ans = std::max(ans, S.query(ranges[l].m_right, ranges[r].m_left).m_sum);
                else if (r != l + 1)
                    ans = std::max(ans, S.query(ranges[l].m_right, ranges[r - 1].m_left).m_sum);
                ans = std::max(ans, S.query(ranges[l].m_left, ranges[l].m_right).m_max);
                mask -= __uint128_t(1) << ranges[l++].m_val;
            }
            cout << ans << endl;
        }
    }
}

int main() {
    solve_czkw();
}