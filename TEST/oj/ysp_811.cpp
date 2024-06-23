#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"

/*
[Number of Subsequences](https://judge.yosupo.jp/problem/number_of_subsequences)(https://github.com/yosupo06/library-checker-problems/issues/811)
*/
/**
 * 本题为简单一个去重 dp
 * 假设从前往后生成每个子序列的生成方式为，如果 sub[i]=a[j],sub[i+1]=a[k], 则必须保证 k 为下标 j 之后出现的第一个值为 a[k] 的位置
 * 基于此假设去重
 */

using mint = OY::mint998244353;
OY::GHASH::UnorderedMap<uint32_t, mint, false, 800003> GS;
int main() {
    uint32_t n;
    cin >> n;
    mint ans = 0;
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        auto [pre, flag] = GS.insert(x);
        mint cur = flag ? ans + 1 : ans - pre->m_mapped;
        pre->m_mapped = ans;
        ans += cur;
    }
    cout << ans;
}