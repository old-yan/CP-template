#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

#include <map>

/*
[P3449 [POI2006] PAL-Palindromes](https://www.luogu.com.cn/problem/P3449)
*/
/**
 * 本题可以使用字符串哈希解决
 * 若 a 为回文串， b 为回文串，那么 a 和 b 的循环节必须相同，才能保证 a + b 也为回文串
 * 而找循环节在 P3538 中有
 */

static constexpr uint32_t N = 2000000;
using mint = OY::mint998244353;
using table_type = OY::STRHASH::SequenceHashPresumTable<mint, 128, N>;
using hash_type = table_type::hash_type;
OY::Eratosthenes::Sieve<N, false, true, false> es;
uint32_t find_pattern(const table_type &S) {
    uint32_t len = S.m_presum.size() - 1;
    for (auto &&[p, c] : es.decomposite(len))
        while (c--) {
            uint32_t pl = len / p;
            if (S.query(0, len - pl - 1) == S.query(pl, len - 1)) len = pl;
        }
    return len;
}
int main() {
    uint32_t n;
    cin >> n;
    std::map<hash_type, uint32_t> counter;
    uint64_t ans = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t m;
        std::string s;
        cin >> m >> s;
        table_type S(s);
        uint32_t pl = find_pattern(S);
        hash_type val = S.query(0, pl - 1);
        ans += ++counter[val] * 2 - 1;
    }
    cout << ans;
}