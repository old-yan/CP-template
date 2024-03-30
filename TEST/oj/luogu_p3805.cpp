#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/Manacher.h"
#include "STR/StrHash.h"

/*
[P3805 【模板】manacher](https://www.luogu.com.cn/problem/P3805)
*/
/**
 * 本题为回文串模板题，可以有多种做法
 * 首先它是一道马拉车模板题
 * 也可以用哈希解决
 */

void solve_manacher() {
    std::string s;
    cin >> s;
    OY::Manacher<true, true> M(s);
    uint32_t ans = 0;
    for (int i = 0; i < s.size(); i++) ans = std::max(ans, M.query_max_odd(i) * 2 + 1);
    for (int i = 1; i < s.size(); i++) ans = std::max(ans, M.query_max_even(i - 1, i) * 2);
    cout << ans << endl;
}

void solve_hash() {
    using mint = OY::mintu32;
    using table_type = OY::STRHASH::StrHashPresumTable<mint, 131>;
    using hash_type = table_type::hash_type;
    std::string s;
    cin >> s;
    hash_type::s_info.prepare_unit(s.size());
    table_type T(s), T2(s.rbegin(), s.rend());
    int odd_ans = 1, even_ans = 0;
    for (int i = 0; i < s.size(); i++) {
        // 先试图延长奇数长度的
        for (int l = i - odd_ans / 2 - 1, r = i + odd_ans / 2 + 1; ~l && r < s.size() && T.query_value(l, r) == T2.query_value(s.size() - r - 1, s.size() - l - 1); l--, r++) odd_ans += 2;
        // 再试图延长偶数长度的
        for (int l = i - even_ans / 2 - 1, r = i + even_ans / 2; ~l && r < s.size() && T.query_value(l, r) == T2.query_value(s.size() - r - 1, s.size() - l - 1); l--, r++) even_ans += 2;
    }
    cout << std::max(odd_ans, even_ans) << endl;
}

int main() {
    solve_manacher();
    // solve_hash();
}