#include "IO/FastIO.h"
#include "STR/ZAlgorithm.h"

/*
[Z Algorithm](https://judge.yosupo.jp/problem/zalgorithm)(https://github.com/yosupo06/library-checker-problems/issues/49)
*/
/**
 * 本题为 z 算法模板
 */

static constexpr uint32_t N = 500000;
int main() {
    std::string s;
    cin >> s;
    auto Z = OY::ZAlgorithm<std::string>(s);
    cout << s.size();
    for (uint32_t i = 1; i != s.size(); i++) cout << ' ' << Z.query_Z(i);
}
