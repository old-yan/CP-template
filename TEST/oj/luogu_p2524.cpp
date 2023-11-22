#include "IO/FastIO.h"
#include "MATH/CantorRank.h"
#include "MATH/StaticModInt32.h"
#include "MATH/StaticMontgomeryModInt32.h"

/*
[P2524 Uim的情人节礼物·其之弐](https://www.luogu.com.cn/problem/P2524)
*/
/**
 * 本题要求的是一个排列的排名，需要康托展开
 */

int main() {
    using mint = OY::StaticModInt32<998244353, true>;
    // using mint = OY::StaticMontgomeryModInt32<998244353, true>;
    uint32_t n;
    cin >> n;
    std::vector<char> arr(n);
    for (auto &a : arr) cin >> a;
    cout << OY::CantorRank<mint>::raw_query(arr.begin(), arr.end()).val() + 1;
}