#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

/*
[#103. 子串查找」三个朋友](https://loj.ac/p/103)
*/
/**
 * 本题可以使用字符串哈希解决
 */

using mint = OY::mint998244353;
using table_type = OY::STRHASH::SequenceHashPresumTable<mint, 128, 1000000>;
using hash_type = table_type::hash_type;
int main() {
    std::string s, t;
    cin >> s >> t;

    table_type S(s);
    hash_type val(t);

    uint32_t ans = 0;
    for (int l = 0, r = t.size() - 1; r != s.size(); l++, r++)
        if (S.query(l, r) == val) ans++;
    cout << ans;
}