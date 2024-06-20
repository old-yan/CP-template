#include "IO/FastIO.h"
#include "STR/Manacher.h"

/*
[Enumerate Palindromes](https://judge.yosupo.jp/problem/enumerate_palindromes)(https://github.com/yosupo06/library-checker-problems/issues/574)
*/
/**
 * 本题为马拉车算法模板
 */

static constexpr uint32_t N = 500000;
int main() {
    std::string s;
    cin >> s;
    auto M = OY::Manacher<true, true>(s);
    for (uint32_t i = 0; i != s.size(); i++) {
        cout << M.query_max_odd(i) * 2 + 1 << ' ';
        if (i + 1 != s.size()) cout << M.query_max_even(i, i + 1) * 2 << ' ';
    }
}
