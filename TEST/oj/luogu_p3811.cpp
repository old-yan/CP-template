#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/DynamicMontgomeryModInt32.h"
#include "MATH/ModularInverseRangeTable.h"

/*
[P3811 【模板】模意义下的乘法逆元](https://www.luogu.com.cn/problem/P3811)
*/
/**
 *本题为逆元范围打表模板题
 */

int main() {
    using mint = OY::DynamicMontgomeryModInt32<0>;
    // using mint = OY::DynamicModInt32<0>;
    uint32_t n, p;
    cin >> n >> p;
    mint::set_mod(p, true);
    OY::ModularInverseRangeTable<mint, 3000000> table(n);
    for (uint32_t i = 1; i <= n; i++) cout << table[i] << endl;
}