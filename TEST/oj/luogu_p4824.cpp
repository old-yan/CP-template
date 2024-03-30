#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/StrHash.h"

/*
[P4824 [USACO15FEB] Censoring S](https://www.luogu.com.cn/problem/P4824)
*/
/**
 * 本题可以使用字符串哈希解决
 */

using mint = OY::mintu64;
using table_type = OY::STRHASH::StrHashPresumTable<mint, 131>;
using hash_type = table_type::hash_type;
int main() {
    std::string s, t;
    cin >> s >> t;
    hash_type::s_info.prepare_unit(s.size());

    auto val = hash_type(t).m_val;

    table_type S;
    std::string res;
    S.reserve(s.size());
    res.reserve(s.size());

    for (char c : s) {
        res.push_back(c);
        S.push_back(c);
        if (res.size() >= t.size() && S.query_value(res.size() - t.size(), res.size() - 1) == val) {
            res.resize(res.size() - t.size());
            S.resize(res.size());
        }
    }

    cout << res;
}