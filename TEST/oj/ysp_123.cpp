#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

/*
[Number of Substrings](https://judge.yosupo.jp/problem/number_of_substrings)(https://github.com/yosupo06/library-checker-problems/issues/123)
*/
/**
 * 本题为后缀数组算法模板
 */

static constexpr uint32_t N = 500000;
int main() {
    std::string s;
    cin >> s;
    uint32_t n = s.size();
    OY::SA::SuffixArray<true, true, N> SA(s.begin(), s.end(), 128);
    uint64_t ans = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t can = n - SA.query_sa(i);
        uint32_t h = SA.query_height(i);
        ans += can - h;
    }
    cout << ans << endl;
}