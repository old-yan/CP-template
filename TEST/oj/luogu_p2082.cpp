#include "DS/RangeManager.h"
#include "IO/FastIO.h"

/*
[P2082 区间覆盖（加强版）](https://www.luogu.com.cn/problem/P2082)
*/
/**
 * 本题为区间管理的模板题
 * RangeManager 自带长度维护，所以不需要传递回调
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::RangeManager<uint64_t> S;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.add_range({l, r});
    }
    cout << S.m_length;
}
