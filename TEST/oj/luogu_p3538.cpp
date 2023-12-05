#include "IO/FastIO.h"
#include "MATH/Eratosthenes.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

/*
[P3538 [POI2012] OKR-A Horrible Poem](https://www.luogu.com.cn/problem/P3538)
*/
/**
 * 本题为字符串哈希模板题
 * 对于每次查询循环节，只要枚举其长度的因子即可
 */

static constexpr uint32_t N = 500000;
using mint = OY::mint1000000007;
using table_type = OY::STRHASH::SequenceHashPresumTable<mint, 128, N>;
using hash_type = table_type::hash_type;
int main() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    table_type S(s);
    OY::Eratosthenes::Sieve<N, false, true, false> es(n);

    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t l, r;
        cin >> l >> r;
        l--, r--;
        uint32_t len = r - l + 1;
        for (auto &&[p, c] : es.decomposite(len))
            while (c--) {
                uint32_t pl = len / p;
                if (S.query(l, l + len - pl - 1) == S.query(l + pl, l + len - 1)) len = pl;
            }
        cout << len << endl;
    }
}