#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/StrHash.h"

/*
[#103. 子串查找」三个朋友](https://loj.ac/p/103)
*/
/**
 * 本题可以使用字符串哈希解决
 */

using mint = OY::mintu32;
using table_type = OY::STRHASH::StrHashPresumTable<mint, 131>;
using hash_type = table_type::hash_type;
int main() {
    std::string s, t;
    cin >> s >> t;

    uint32_t max_len = std::max(s.size(), t.size());
    hash_type::s_info.prepare_unit(max_len);

    // 计算 t 的哈希值
    auto val = hash_type(t).m_val;

    // 把 s 的哈希值前缀表打出来
    table_type S(s);
    uint32_t ans = 0;
    // 对每个相应长度的子串进行检查
    // 因为长度已经相等了，所以只比较值
    for (int l = 0, r = t.size() - 1; r != s.size(); l++, r++)
        if (S.query_value(l, r) == val) ans++;
    cout << ans;
}