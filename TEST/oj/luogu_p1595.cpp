#include "IO/FastIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt64.h"

/*
[P1595 信封问题](https://www.luogu.com.cn/problem/P1595)
*/
/**
 * 本题为错排数模板题
 */

int main() {
    using mint = OY::mint9223372036854775783;

    uint32_t n;
    cin >> n;
    OY::CombinationTable<mint, 20> table(n);

    cout << table.perm_staggered(n) << endl;
}