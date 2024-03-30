#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/HashLCP.h"
#include "STR/MinimalSequence.h"

/*
[P1709 [USACO5.5] 隐藏口令 Hidden Password](https://www.luogu.com.cn/problem/P1709)
*/
/**
 * 本题可以有多种解法
 * 首先，最小表示法标准做法可以有 O(n) 的小常数做法
 * 显然可以用哈希做
 */

void solve_minimal_sequence() {
    uint32_t n;
    cin >> n;
    std::string s;
    s.reserve(n);
    while (s.size() < n) {
        std::string tmp;
        cin >> tmp;
        s += tmp;
    }
    uint32_t i = OY::get_minimal_rotation(s.begin(), s.end());
    cout << i << endl;
}

void solve_hash() {
    using mint = OY::mintu32;
    using lcp_type = OY::HASHLCP::LCP<std::vector<uint32_t>, mint, 131>;
    using hash_type = lcp_type::table_type::hash_type;
    uint32_t n;
    cin >> n;
    hash_type::s_info.prepare_unit(n * 2);
    std::string s;
    s.reserve(n);
    while (s.size() < n) {
        std::string tmp;
        cin >> tmp;
        s += tmp;
    }
    s.insert(s.end(), s.begin(), s.end());
    lcp_type LCP(s.begin(), s.end());

    uint32_t ans = 0;
    for (uint32_t i = 1; i < n; i++)
        if (LCP.compare(i, i + n - 1, ans, ans + n - 1) < 0) ans = i;
    cout << ans << endl;
}

int main() {
    solve_minimal_sequence();
    // solve_hash();
}
