#include "DS/KBIT.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
// #include "MATH/StaticMontgomeryModInt32.h"

/*
[牛牛的等差数列](https://ac.nowcoder.com/acm/problem/204829)
*/
/**
 * 考虑一个数组 a 的一阶、二阶、三阶前缀和分别为 S1, S2, S3
 * a 的单点修改，等价于 S1 的区间常量修改，等价于 S2 的区间等差数列修改
 * S2 的区间和，等价于 S3 的单点查询的差分
 * 在这道题里，S2 就是我们要维护的数组
 */

void solve_kbit() {
    // 111546435 是 3~25 之间所有素数的 lcm，乘个 9 让模数大于 1e9
    using mint = OY::StaticModInt32<111546435 * 9, false>;
    // using mint = OY::StaticMontgomeryModInt32<111546435 * 9, false>;
    using KBIT = OY::KBIT::Tree<mint, 3, 200000>;
    KBIT::prepare();

    uint32_t n;
    cin >> n;
    // 用 KBIT 维护数组 a，及其最高三阶前缀和
    // 注意输入的是数组 a 的二阶前缀和 S2
    std::vector<mint> a(n);
    for (auto &e : a) cin >> e;
    // 获取 S1
    std::adjacent_difference(a.begin(), a.end(), a.begin());
    // 获取 a
    std::adjacent_difference(a.begin(), a.end(), a.begin());
    KBIT S(a.begin(), a.end());

    uint32_t m;
    cin >> m;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            // 二阶前缀和 S2 区间加一个一次函数
            uint32_t l, r;
            mint val, d;
            cin >> l >> r >> val >> d;
            // y = d*x + (val - d*(l-1))
            S.add<2, 2>(l - 1, r - 1, {val - d * (l - 1), d});
        } else {
            uint32_t l, r, m;
            cin >> l >> r >> m;
            auto res = S.query<2>(l - 1, r - 1).val();
#define IF_M(x)                  \
    case x:                      \
        cout << res % x << endl; \
        break
            switch (m) {
                IF_M(3);
                IF_M(5);
                IF_M(7);
                IF_M(11);
                IF_M(13);
                IF_M(17);
                IF_M(19);
                IF_M(23);
            }
#undef IF_M
        }
    }
}

int main() {
    solve_kbit();
}
