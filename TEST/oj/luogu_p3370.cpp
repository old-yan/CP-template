#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/StrHash.h"

/*
[P3370 【模板】字符串哈希](https://www.luogu.com.cn/problem/P3370)
*/
/**
 * 本题为字符串哈希模板题
 * 检查字符串种类数，也就是检查哈希结果种类数
 */

using mint = OY::mintu32;
using hash_type = OY::STRHASH::StrHash<mint, 131>;
int main() {
    hash_type::s_info.prepare_unit(1500);
    uint32_t n;
    cin >> n;
    std::vector<hash_type> a;
    a.reserve(n);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        a.emplace_back(s);
    }
    std::sort(a.begin(), a.end());
    a.erase(std::unique(a.begin(), a.end()), a.end());
    cout << a.size();
}