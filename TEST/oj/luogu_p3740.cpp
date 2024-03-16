#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P3740 [HAOI2014] 贴海报](https://www.luogu.com.cn/problem/P3740)
*/
/**
 * 本题为 RangeManager 模板题
 * 倒序插入每个区间，如果某个区间插入时，该区间已经全部存在，说明这个区间最终被完全覆盖
 */

void solve_rm() {
    uint32_t n, m;
    cin >> n >> m;
    std::vector<std::pair<uint32_t, uint32_t>> colors(m);
    for (uint32_t i = 0; i < m; i++) {
        auto &[l, r] = colors[i];
        cin >> l >> r;
    }
    OY::RangeManager<uint32_t> rm;
    uint32_t ans = 0;
    for (uint32_t i = m - 1; ~i; i--)
        if (rm.all_of(colors[i]) == rm.end()) {
            ans++;
            rm.add_range(colors[i]);
        }
    cout << ans;
}

int main() {
    solve_rm();
}