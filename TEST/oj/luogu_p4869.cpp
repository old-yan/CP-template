#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"
#include "MATH/StaticModInt32.h"

/*
[P4869 albus就是要第一个出场](https://www.luogu.com.cn/problem/P4869)
*/
/**
 * 本题为线性基模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::StaticHamelXorBase32<30> hxb{};
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        hxb.insert(x);
    }

    using mint = OY::StaticModInt32<10086, false>;
    uint32_t q;
    cin >> q;
    cout << hxb.rank(q) * mint(2).pow(n - hxb.base_count()) + 1;
}