#include "DS/DynamicBitset.h"
#include "DS/StaticBitset.h"
#include "IO/FastIO.h"

/*
[P5020 [NOIP2018 提高组] 货币系统](https://www.luogu.com.cn/problem/P5020)
*/
/**
 * 通过完全背包，求解每个数字是否能被其他数字表示
 */

void solve_bitset() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n;
        cin >> n;
        std::vector<uint32_t> a(n);
        for (uint32_t i = 0; i != n; i++) cin >> a[i];
        std::ranges::sort(a);

        uint32_t ans = 0;
        OY::DynamicBitset S(a.back() + 1);
        // OY::StaticBitset<25001> S{};
        S.set(0);
        for (auto e : a)
            if (!S[e]) {
                ans++;
                S.bitor_lshift_unbounded(e);
            }
        cout << ans << endl;
    }
}

int main() {
    solve_bitset();
}