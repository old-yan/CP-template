#include "IO/FastIO.h"
#include "MATH/CantorRank.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P5367 【模板】康托展开](https://www.luogu.com.cn/problem/P5367)
*/
/**
 * 本题要求的是一个排列的排名，需要康托展开
 */

int main() {
    using mint = OY::mint998244353;
    // using mint = OY::mgint998244353;
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) cin >> a;
    auto ans = OY::CantorRank<mint>::raw_query(arr.data(), arr.data() + n);
    cout << ans + 1;
}