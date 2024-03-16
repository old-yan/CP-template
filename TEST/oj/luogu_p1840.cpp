#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P1840 Color the Axis](https://www.luogu.com.cn/problem/P1840)
*/
/**
 * 本题可以使用 RangeManager 解决
 */

void solve_rm() {
    uint32_t n, m;
    cin >> n >> m;
    OY::RangeManager<uint32_t> black;
    black.add_range({1, n});
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        black.remove_range({l, r});
        cout << black.m_length << endl;
    }
}

int main() {
    solve_rm();
}
