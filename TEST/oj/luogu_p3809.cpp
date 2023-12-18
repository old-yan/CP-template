#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/HashLCP.h"
#include "STR/SuffixArray.h"

/*
[P3809 【模板】后缀排序](https://www.luogu.com.cn/problem/P3809)
*/
/**
 * 本题为后缀数组模板题，可以有多种做法
 * 可以使用哈希二分来实现
 */

void solve_SA() {
    std::string s;
    cin >> s;
    OY::SA::SuffixArray<false, false, 1000000> SA(s);
    for (uint32_t i = 0; i < s.size(); i++) cout << SA.query_sa(i) + 1 << ' ';
}

void solve_hash() {
    using mint = OY::mint998244353;
    using lcp_type = OY::HASHLCP::LCP<std::string, mint, 128, 1000000>;
    using hash_table = lcp_type::table_type;
    using hash_type = hash_table::hash_type;
    std::string s;
    cin >> s;
    lcp_type LCP(s);

    uint32_t n = s.size();
    std::vector<uint32_t> indexes(n);
    std::iota(indexes.begin(), indexes.end(), 0);
    std::stable_sort(indexes.begin(), indexes.end(), [&](uint32_t x, uint32_t y) {
        if (s[x] < s[y]) return true;
        if (s[x] > s[y]) return false;
        if (s[x + 1] < s[y + 1]) return true;
        if (s[x + 1] > s[y + 1]) return false;
        return LCP.compare(x, n - 1, y, n - 1) < 0;
    });
    for (auto i : indexes) cout << i + 1 << ' ';
}

int main() {
    solve_SA();
    // solve_hash();
}