#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/LucasTable_ex.h"

/*
[P4720 【模板】扩展卢卡斯定理/exLucas](https://www.luogu.com.cn/problem/P4720)
*/
/**
 * 本题为扩展卢卡斯定理模板题，模数随意
 */

using mint = OY::DynamicModInt32<0>;
int main() {
    uint64_t n, m;
    uint32_t p;
    cin >> n >> m >> p;
    mint::set_mod(p, false);
    cout << OY::LucasTable_ex<mint>().query(n, m) << endl;
}
