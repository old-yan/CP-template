#include "DS/KBIT.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P4458 [BJOI2018] 链上二次求和](https://www.luogu.com.cn/problem/P4458)
*/
/**
 * 以下按照下标从 1~n 来考虑
 * 假设原数组的差分为数组 a，原数组即为 a 的一阶前缀和 S1
 * S1 的前缀和表为 S2(也就是 a 的二阶前缀和)
 * S2 的前缀和表为 S3(也就是 a 的三阶前缀和)
 * S3 的前缀和表为 S4(也就是 a 的四阶前缀和)
 * 查询内容为
 *              Σ_{len=l}^{r} Σ_{i=len}^{n} S1.query(i-len+1,i);
 * S1 的区间查询可以转换为 S2 的差分：
 *              Σ_{len=l}^{r} Σ_{i=len}^{n} (S2.query(i)-S2.query(i-len));
 * 对 S2 求 ∑，可以转换为 S3 的差分：
 *              Σ_{len=l}^{r} Σ_{i=len}^{n} S2.query(i) - Σ_{len=l}^{r} Σ_{i=len}^{n} S2.query(i-len);
 *              Σ_{len=l}^{r} (S3.query(n)-S3.query(len-1)) - Σ_{len=l}^{r} S3.query(n-len)
 *              (r-l+1)*S3.query(n) - Σ_{len=l}^{r} S3.query(len-1) - Σ_{len=l}^{r} S3.query(n-len)
 * 对 S3 求 ∑，可以转换为 S4 的差分：
 *              (r-l+1)*S3.query(n) - (S4.query(r-1)-S4.query(l-2)) - (S4.query(n-l)-S4.query(n-1-r))
 * 至此，查询内容已经转化为 S3 和 S4 的点查询
 */

void solve_kbit() {
    using mint = OY::mgint1000000007;
    // using mint = OY::mint1000000007;
    using KBIT = OY::KBIT::Tree<mint, 4, 200001>;
    KBIT::prepare();

    uint32_t n, m;
    cin >> n >> m;
    // 用 KBIT 维护数组 a，及其最高四阶前缀和
    // 注意输入的是 S1
    mint lst{};
    KBIT S(n + 1, [&](uint32_t i) -> mint {
        if (!i) return 0;
        mint x;
        cin >> x;
        std::swap(x, lst);
        return lst - x;
    });

    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            // 一阶前缀和 S1 区间加一个常数
            uint32_t l, r;
            mint val;
            cin >> l >> r >> val;
            if (l > r) std::swap(l, r);
            S.add<1, 1>(l, r, {val});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            if (l > r) std::swap(l, r);
            auto v1 = S.query<3>(n);
            auto v2 = S.query<4>(r - 1);
            auto v3 = S.query<4>(l - 2);
            auto v4 = S.query<4>(n - l);
            auto v5 = S.query<4>(n - 1 - r);
            auto res = mint::raw(r - l + 1) * v1 - (v2 - v3) - (v4 - v5);
            cout << res << endl;
        }
    }
}

int main() {
    solve_kbit();
}
