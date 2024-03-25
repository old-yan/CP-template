#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P2434 [SDOI2005] 区间](https://www.luogu.com.cn/problem/P2434)
*/
/**
 * 本题为区间管理的模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::RangeManager<uint32_t> S;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.add_range({l, r - 1});
    }
    for (auto &[l, r] : S)
        cout << l << ' ' << r + 1 << '\n';
}
