#include "DS/KBIT.h"
#include "DS/LinkBucket.h"
#include "IO/FastIO.h"
#include "MATH/StaticMontgomeryModInt64.h"

/*
[P4062 [Code+#1] Yazid 的新生舞会](https://www.luogu.com.cn/problem/P4062)
*/
/**
 * 考虑元素 x 作为绝对众数的贡献。
 * 令每个位置的权值为 0 （元素非 x）或者 1（元素为 x），则：
 *      区间 [l, r] 满足 presum(r)-presum(l-1)> r-l+1-(presum(r)-presum(l-1))
 *      即，presum(r)*2-r > presum(l-1)*2-(l-1)
 *      若定义 f(i)=presum(i)*2-i，则为 f(r) > f(l-1)
 * 所以只要从左往右扫描维护 f 函数，就可以计算出元素 x 作为绝对众数的贡献
 * 然而，元素 x 在整个数组里的出现次数并不多，它可能是稀疏出现的，所以必须一段一段地去考虑
 * 把 x 出现的位置做为段的起始点，则每段段内的 f 函数均为一个递降序列，所以对 f 函数的维护可以通过连续区间的加实现
 *
 * 定义 g[i] 为满足 f 值为 i 的点数，则上述一个段的维护就是 g 的区间加一
 * 而段的贡献就是 f 的一阶前缀和表的区间和，也就是 f 的二阶前缀和表的差分
 *
 * 我们可以维护 g 的差分，及 g 的差分的三阶前缀和
 */

static constexpr uint32_t N = 500000;
void solve_kbit() {
    using mint = OY::mgint4611686018427387847;
    using KBIT = OY::KBIT::Tree<mint, 3, N * 2 + 1>;
    KBIT::prepare();

    uint32_t n, type;
    cin >> n >> type;

    OY::LBC::LinkBucket<uint32_t, N, N> buckets(n, n);
    // 注意 LinkBucket 是倒着存的，那我们干脆认为数组是反着的
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t x;
        cin >> x;
        buckets[x].push_back(n + 1 - i);
    }

    // 用 KBIT 维护数组 g 的差分，及其最高三阶前缀和
    // 由于 g 的值域是 [-n, n]，所以统一偏移 n，偏移到 [0, n*2]
    KBIT S(n * 2 + 1);
    mint ans{};
    // 由于对每个 x 开一颗新树是不现实的，所以复用同一颗 KBIT
    for (uint32_t x = 0; x != n; x++)
        if (!buckets[x].empty()) {
            // lst, lst_val 分别表示上一段的初始位置、初值
            uint32_t lst = 0, lst_val = 0;
            for (auto pos : buckets[x]) {
                // 上一段的初值为 lst_val ，末值为 lst_val - (pos-lst) + 1
                uint32_t lst_start = lst_val, lst_end = lst_val - (pos - lst) + 1;
                // 计算上一段与更早的 g 形成的贡献
                // 记得要偏移 n
                ans += S.query<2>(lst_end - 1 + n, lst_start - 1 + n);
                // 把上一段加到树状数组里
                // 记得要偏移 n
                S.add<1, 1>(lst_end + n, lst_start + n, {mint::raw(1)});
                lst_val = lst_end + 1, lst = pos;
            }
            // 最后记得算一下最后一段的贡献
            uint32_t lst_start = lst_val, lst_end = lst_val - (n + 1 - lst) + 1;
            // 计算上一段与更早的 g 形成的贡献
            // 记得要偏移 n
            ans += S.query<2>(lst_end - 1 + n, lst_start - 1 + n);

            // 通过撤销每一次操作，清零树状数组
            lst = 0, lst_val = 0;
            for (auto pos : buckets[x]) {
                uint32_t lst_start = lst_val, lst_end = lst_val - (pos - lst) + 1;
                S.add<1, 1>(lst_end + n, lst_start + n, {-mint::raw(1)});
                lst_val = lst_end + 1, lst = pos;
            }
        }
    cout << ans << endl;
}

int main() {
    solve_kbit();
}