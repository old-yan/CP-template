#include "IO/FastIO.h"
#include "MATH/Euclidean.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P5170 【模板】类欧几里得算法](https://www.luogu.com.cn/problem/P5170)
*/
/**
 * 本题为类欧几里得算法模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    using mint = OY::mint998244353;
    // using mint = OY::mgint998244353;
    OY::Euclidean<mint> ec;
    while (t--) {
        uint32_t n, a, b, c;
        cin >> n >> a >> b >> c;
        auto [f, g, h] = ec.calc(a, b, c, n);
        cout << f << ' ' << h << ' ' << g << '\n';
    }
}