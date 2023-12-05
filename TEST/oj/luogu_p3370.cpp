#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

/*
[P3370 【模板】字符串哈希](https://www.luogu.com.cn/problem/P3370)
*/
/**
 * 本题为字符串哈希模板题
 * 检查字符串种类数，也就是检查哈希结果种类数
*/

using mint = OY::mint998244353;
using hash_type = OY::STRHASH::SequenceHash<mint, 128, 1000>;
int main() {
    uint32_t n;
    cin >> n;
    std::vector<hash_type> a;
    a.reserve(n);
    for (uint32_t i = 0; i < n; i++) {
        std::string s;
        cin >> s;
        a.push_back(hash_type(s));
    }
    std::sort(a.begin(), a.end());
    a.erase(std::unique(a.begin(), a.end()), a.end());
    cout << a.size();
}