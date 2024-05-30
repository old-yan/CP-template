#include "DS/OfflineMode.h"
#include "DS/RangeMode.h"
#include "IO/FastIO.h"

/*
[P3709 大爷的字符串题](https://www.luogu.com.cn/problem/P3709)
*/
/**
 * 本题需要对静态区间进行区间众数查询
 */

void solve_offline() {
    uint32_t n, m;
    cin >> n >> m;
    OY::OFFLINEMODE::Solver<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x; }, m);
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        S.add_query(l - 1, r - 1);
    }
    for (auto a : S.solve<OY::OFFLINEMODE::ELEM_FREQUENCY>()) cout << '-' << a.m_freq << endl;
}

void solve_mode() {
    uint32_t n, m;
    cin >> n >> m;
    OY::RangeMode<uint32_t> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << '-' << S.query(l - 1, r - 1).m_cnt << endl;
    }
}

int main() {
    solve_offline();
    // solve_mode();
}