#include "IO/FastIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[U177764 【模板】错排统计/逃脱](https://www.luogu.com.cn/problem/U177764)
*/
/**
 * 本题为错位排列数模板题
 */

int main() {
    using mint = OY::mint998244353;
    // using mint = OY::mgint998244353;
    OY::CombinationTable<mint, 100000> table(100000);
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        cout << table.perm_staggered(x) << endl;
    }
}