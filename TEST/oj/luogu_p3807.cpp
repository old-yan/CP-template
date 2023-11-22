#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/DynamicMontgomeryModInt32.h"
#include "MATH/LucasTable.h"

/*
[P3807 【模板】卢卡斯定理/Lucas 定理](https://www.luogu.com.cn/problem/P3807)
*/
/**
 * 本题为卢卡斯定理模板题，要求模数为质数
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        using mint = OY::DynamicModInt32<0>;
        // using mint = OY::DynamicMontgomeryModInt32<0>;
        uint32_t n, m, p;
        cin >> n >> m >> p;
        mint::set_mod(p, true);
        cout << OY::LucasTable<mint>().query(n + m, m) << endl;
    }
}
