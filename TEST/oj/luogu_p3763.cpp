#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

/*
[P3763 [TJOI2017] DNA](https://www.luogu.com.cn/problem/P3763)
*/
/**
 * 本题可以使用字符串哈希解决
 * 通过哈希，可以快速求出子串 LCP；最多找四次 LCP 可以完成本题的检查任务
 */

static constexpr uint32_t N = 100000;
using mint = OY::mint998244353;
using table_type = OY::STRHASH::SequenceHashPresumTable<mint, 128, N>;
int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        std::string s1, s2;
        cin >> s1 >> s2;
        table_type table1(s1);
        table_type table2(s2);
        uint32_t ans = 0;
        for (uint32_t l = 0, r = s2.size() - 1; r < s1.size(); l++, r++) {
            uint32_t cur1 = l, cur2 = 0;
            auto lcp = table_type::lcp(table1, cur1, table2, cur2);
            cur1 += lcp, cur2 += lcp;
            if (cur2 == s2.size()) {
                ans++;
                continue;
            }
            cur1++, cur2++;
            lcp = table_type::lcp(table1, cur1, table2, cur2);
            cur1 += lcp, cur2 += lcp;
            if (cur2 == s2.size()) {
                ans++;
                continue;
            }
            cur1++, cur2++;
            lcp = table_type::lcp(table1, cur1, table2, cur2);
            cur1 += lcp, cur2 += lcp;
            if (cur2 == s2.size()) {
                ans++;
                continue;
            }
            cur1++, cur2++;
            lcp = table_type::lcp(table1, cur1, table2, cur2);
            cur1 += lcp, cur2 += lcp;
            if (cur2 == s2.size()) ans++;
        }
        cout << ans << endl;
    }
}