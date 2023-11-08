#include "DS/RangeManager.h"
#include "IO/FastIO.h"

#include <set>

/*
[P2434 [SDOI2005] 区间](https://www.luogu.com.cn/problem/P2434)
*/
/**
 * 本题为区间管理的模板题
 * 展示一下回调函数的用法
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::RangeManager<uint64_t> S;
    std::set<std::pair<uint32_t, uint32_t>> ans;
    // 添加区间时的回调函数
    auto add_call = [&](uint32_t l, uint32_t r) {
        ans.emplace(l, r);
    };
    // 移除区间时的回调函数
    auto remove_call = [&](uint32_t l, uint32_t r) {
        ans.erase({l, r});
    };
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.add_range({l, r - 1}, add_call, remove_call);
    }
    for (auto &[l, r] : ans)
        cout << l << ' ' << r + 1 << '\n';
}
