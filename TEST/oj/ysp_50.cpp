#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

/*
[Suffix Array](https://judge.yosupo.jp/problem/suffixarray)(https://github.com/yosupo06/library-checker-problems/issues/50)
*/
/**
 * 本题为后缀数组模板
 */

static constexpr uint32_t N = 500000;
int main() {
    std::string s;
    cin >> s;
    auto SA = OY::SA::SuffixArray<false, false, N>(s);
    for (uint32_t i = 0; i != s.size(); i++) cout << SA.query_sa(i) << ' ';
}
