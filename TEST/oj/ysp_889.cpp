#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

/*
[Longest Common Substring](https://judge.yosupo.jp/problem/longest_common_substring)(https://github.com/yosupo06/library-checker-problems/issues/889)
*/
/**
 * 本题建出后缀数组即可解决
 */

static constexpr uint32_t N = 500000;
int main() {
    std::string s, t;
    cin >> s >> t;
    uint32_t n1 = s.size(), n2 = t.size();
    OY::SA::SuffixArray<true, true, N * 2> SA(n1 + n2 + 1, [&](uint32_t i) {
        return i <= n1 ? s[i] : t[i - n1 - 1];
    });
    uint32_t ans = 0, a{}, c{};
    for (uint32_t i = 2; i != n1 + n2 + 1; i++)
        if ((SA.query_sa(i - 1) < n1) != (SA.query_sa(i) < n1) && SA.query_height(i) > ans) {
            ans = SA.query_height(i);
            if (SA.query_sa(i - 1) < n1)
                a = SA.query_sa(i - 1), c = SA.query_sa(i) - n1 - 1;
            else
                a = SA.query_sa(i), c = SA.query_sa(i - 1) - n1 - 1;
        }
    cout << a << ' ' << a + ans << ' ' << c << ' ' << c + ans;
}