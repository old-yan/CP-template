#include "IO/FastIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P4931 [MtOI2018] 情侣？给我烧了！（加强版）](https://www.luogu.com.cn/problem/P4931)
*/
/**
 * 本题为 p4921 数据加强版
 */

int main() {
    using mint = OY::mint998244353;
    // using mint = OY::mgint998244353;
    OY::CombinationTable<mint, 5000000> table(5000000);

    // 预处理出 dp[i] 表示 i 对情人坐 i 行座位的乱排数
    std::vector<mint> dp(5000001);
    dp[0] = 1;
    dp[1] = 0;
    for (uint32_t i = 2; i < dp.size(); i++) {
        dp[i] = (dp[i - 1] + dp[i - 2] * mint::raw(i * 2 - 2)) * mint::raw(4 * i) * mint::raw(i - 1);
    }

    uint32_t t;
    cin >> t;
    for (uint32_t i = 0; i < t; i++) {
        uint32_t n, k;
        cin >> n >> k;
        // 选出配对的 k 对情人
        auto s1 = table.comb(n, k);
        // 选出配对的 k 行座位
        auto s2 = table.perm(n, k);
        // 情人可以左右换位置
        auto s3 = mint::raw(2).pow(k);

        cout << s1 * s2 * s3 * dp[n - k] << endl;
    }
}