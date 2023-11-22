#include "IO/FastIO.h"
#include "MATH/CatalanNumber.h"
#include "MATH/StaticModInt64.h"
#include "MATH/StaticMontgomeryModInt64.h"

/*
[P1044 [NOIP2003 普及组] 栈](https://www.luogu.com.cn/problem/P1044)
*/
/**
 *本题为卡特兰数模板题
 */

int main() {
    using mint = OY::StaticModInt64<9223372036854775783, true>;
    // using mint = OY::StaticMontgomeryModInt64<9223372036854775783, true>;
    uint32_t n;
    cin >> n;
    OY::CatalanNumber<mint, 18> table(n);
    cout << table[n];
}