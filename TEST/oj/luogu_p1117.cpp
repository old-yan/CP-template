#include "DS/AdjDiff.h"
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/HashLCP.h"
#include "STR/SALCP.h"

/*
[P1117 [NOI2016] 优秀的拆分](https://www.luogu.com.cn/problem/P1117)
*/
/**
 * 本题需要用到一个技巧：
 * 当寻找长为 len*2 的重复区间时，可以选两个差为 len 的点，算出向前、向后的公共长度，即可批量找出包含这两个点的长为 len*2 的重复区间
 * 计算 lcp 可以用哈希，也可以用后缀数组
 */

void solve_SA(std::string &s) {
    uint32_t n = s.size();
    OY::SALCP::LCP<600000> SA(s), SA2(s.rbegin(), s.rend());
    OY::AdjDiff::Table<uint32_t, false> S1(n), S2(n);
    S1.switch_to_difference();
    S2.switch_to_difference();
    for (uint32_t len = 1; len * 2 < n; len++) {
        for (uint32_t p1 = 0, p2 = len; p2 < n; p1 += len, p2 += len) {
            uint32_t lcp = SA.lcp(p1, p2, std::min(n - p2, len));
            uint32_t lcs = SA2.lcp(n - p1 - 1, n - p2 - 1, std::min(p1 + 1, len));
            if (lcp + lcs > len) {
                if (lcs > 1) {
                    uint32_t l2 = std::min(lcp, len - 1);
                    S1.add(p1 + 1 - lcs, p1 + l2 - len, 1);
                    S2.add(p1 + len * 2 - lcs, p1 + l2 + len - 1, 1);
                }
                if (lcp >= len) {
                    S1.add(p1, p1, 1);
                    S2.add(p1 + len * 2 - 1, p1 + len * 2 - 1, 1);
                }
            }
        }
    }
    S1.switch_to_value();
    S2.switch_to_value();
    uint64_t ans = 0;
    for (uint32_t i = 1; i < n; i++) ans += uint64_t(S2.query(i - 1)) * S1.query(i);
    cout << ans << endl;
}

void solve_hash(std::string &s) {
    using mint = OY::mint998244353;
    using lcp_type = OY::HASHLCP::LCP<std::string, mint, 128>;
    using hash_type = lcp_type::table_type::hash_type;
    if (!hash_type::s_info.m_u) hash_type::s_info.prepare_unit(30000);
    uint32_t n = s.size();
    lcp_type L1(s), L2(s.rbegin(), s.rend());
    OY::AdjDiff::Table<uint32_t, false> S1(n), S2(n);
    S1.switch_to_difference();
    S2.switch_to_difference();
    for (uint32_t len = 1; len * 2 < n; len++) {
        for (uint32_t p1 = 0, p2 = len; p2 < n; p1 += len, p2 += len) {
            uint32_t lcp = L1.lcp(p1, p2, std::min(n - p2, len));
            uint32_t lcs = L2.lcp(n - p1 - 1, n - p2 - 1, std::min(p1 + 1, len));
            if (lcp + lcs > len) {
                if (lcs > 1) {
                    uint32_t l2 = std::min(lcp, len - 1);
                    S1.add(p1 + 1 - lcs, p1 + l2 - len, 1);
                    S2.add(p1 + len * 2 - lcs, p1 + l2 + len - 1, 1);
                }
                if (lcp >= len) {
                    S1.add(p1, p1, 1);
                    S2.add(p1 + len * 2 - 1, p1 + len * 2 - 1, 1);
                }
            }
        }
    }
    S1.switch_to_value();
    S2.switch_to_value();
    uint64_t ans = 0;
    for (uint32_t i = 1; i < n; i++) ans += uint64_t(S2.query(i - 1)) * S1.query(i);
    cout << ans << endl;
}

int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        solve_SA(s);
        // solve_hash(s);
    }
}