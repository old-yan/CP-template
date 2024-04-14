#include "DS/KBIT.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[智乃酱的平方数列](https://ac.nowcoder.com/acm/problem/226172)
*/
/**
 * 考虑一个数组 a 的一阶、二阶、三阶前缀和分别为 S1, S2, S3, S4
 * a 的单点修改，等价于 S1 的区间常量修改，等价于 S2 的区间等差数列修改，等价于 S3 的区间二次函数修改
 * S3 的区间和，等价于 S4 的单点查询的差分
 * 在这道题里，S3 就是我们要维护的数组
 */

void solve_kbit() {
    using mint = OY::mgint1000000007;
    // using mint = OY::mint1000000007;
    using KBIT = OY::KBIT::Tree<mint, 4, 500000>;
    KBIT::prepare();

    uint32_t n, m;
    cin >> n >> m;
    // 用 KBIT 维护数组 a，及其最高三阶前缀和
    KBIT S(n);

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            // 三阶前缀和 S3 区间加一个二次函数
            uint32_t l, r;
            cin >> l >> r;
            // y = (x- (l-2))^2
            S.add<3, 3>(l - 1, r - 1, {mint(l) * l - l * 4 + 4, 2 * (mint(2) - l), 1});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query<3>(l - 1, r - 1) << endl;
        }
    }
}

int main() {
    solve_kbit();
}
