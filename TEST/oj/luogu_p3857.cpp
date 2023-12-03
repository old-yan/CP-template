#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

/*
[P3857 [TJOI2008] 彩灯](https://www.luogu.com.cn/problem/P3857)
*/
/**
 * 本题为线性基模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::DynamicHamelXorBase64::set_width(n);
    OY::DynamicHamelXorBase64 hxb;
    for (uint32_t i = 0; i < m; i++) {
        std::string s;
        cin >> s;
        uint64_t val = 0;
        for (uint32_t j = 0; j < n; j++)
            if (s[j] == 'O') val |= uint64_t(1) << j;
        hxb.insert(val);
    }
    cout << hxb.total_count() % 2008;
}