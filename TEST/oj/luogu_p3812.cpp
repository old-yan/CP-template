#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

/*
[P3812 【模板】线性基](https://www.luogu.com.cn/problem/P3812)
*/
/**
 * 本题为线性基模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::StaticHamelXorBase64<50> hxb{};
    for (uint32_t i = 0; i < n; i++) {
        uint64_t x;
        cin >> x;
        hxb.insert(x);
    }
    cout << hxb.query_max_bitxor();
}