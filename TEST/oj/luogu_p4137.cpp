#include "DS/OfflineRangeMex.h"
#include "DS/RangeMex.h"
#include "IO/FastIO.h"

/*
[P4137 Rmq Problem / mex](https://www.luogu.com.cn/problem/P4137)
*/
/**
 * 离线求区间 mex 模板
 */

static constexpr uint32_t N = 200000, M = 200000;
uint32_t arr[N];
void solve_offline() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    OY::OfflineRangeMexSolver sol(n, q);
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r;
        cin >> l >> r;
        sol.add_query(l - 1, r - 1);
    }
    auto ans = sol.solve(arr, arr + n);
    for (uint32_t i = 0; i != q; i++) cout << ans[i] << endl;
}

void solve_online() {
    uint32_t n, q;
    cin >> n >> q;
    OY::StaticRangeMex<5000000> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << endl;
    }
}

int main() {
    solve_offline();
    // solve_online();
}