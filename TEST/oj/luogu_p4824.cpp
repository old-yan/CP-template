#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

/*
[P4824 [USACO15FEB] Censoring S](https://www.luogu.com.cn/problem/P4824)
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

    hash_type val(t);

    table_type S;
    std::string res;
    S.reserve(s.size());
    res.reserve(s.size());

    for (char c : s) {
        S.push_back(c);
        res.push_back(c);
        if (res.size() >= t.size() && S.query(res.size() - t.size(), res.size() - 1) == val)
            for (uint32_t i = t.size(); i--;) S.pop_back(), res.pop_back();
    }
    
    cout<<res;
}