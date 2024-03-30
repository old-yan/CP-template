#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/StrHash.h"

#include <set>

/*
[P4407 [JSOI2009] 电子字典](https://www.luogu.com.cn/problem/P4407)
*/
/**
 * 本题可以使用字符串哈希解决
 */

using mint = OY::mintu64;
using table_type = OY::STRHASH::StrHashPresumTable<mint, 131>;
using hash_type = table_type::hash_type;
int main() {
    hash_type::s_info.prepare_unit(20), hash_type::s_info.prepare_unit_inv(20);
    uint32_t n, m;
    cin >> n >> m;
    std::set<hash_type> dict;
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        dict.insert(s);
    }
    for (uint32_t i = 0; i < m; i++) {
        std::string s;
        cin >> s;
        if (dict.contains(s))
            cout << "-1\n";
        else {
            table_type table(s);
            std::vector<hash_type> res;
            // 先查找删除一个字符后的情况
            for (uint32_t i = 0; i < s.size(); i++) {
                auto l = table.query_hash(0, i - 1), r = table.query_hash(i + 1, s.size() - 1);
                auto l_r = l.append_right(r);
                if (dict.contains(l_r)) res.push_back(l_r);
            }
            // 再查找替换一个字符后的情况
            for (uint32_t i = 0; i < s.size(); i++) {
                auto l = table.query_hash(0, i - 1), r = table.query_hash(i + 1, s.size() - 1);
                for (char middle = 'a'; middle <= 'z'; middle++)
                    if (middle != s[i]) {
                        auto x = hash_type::single(middle);
                        auto l_x_r = l.append_right(x).append_right(r);
                        if (dict.contains(l_x_r)) res.push_back(l_x_r);
                    }
            }
            // 再查找添加一个字符后的情况
            for (uint32_t i = 0; i <= s.size(); i++) {
                auto l = table.query_hash(0, i - 1), r = table.query_hash(i, s.size() - 1);
                for (char middle = 'a'; middle <= 'z'; middle++) {
                    auto x = hash_type::single(middle);
                    auto l_x_r = l.append_right(x).append_right(r);
                    if (dict.contains(l_x_r)) res.push_back(l_x_r);
                }
            }
            std::sort(res.begin(), res.end());
            cout << std::unique(res.begin(), res.end()) - res.begin() << endl;
        }
    }
}