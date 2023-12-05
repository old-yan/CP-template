#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"

/*
[#2823. 「BalticOI 2014 Day 1」三个朋友](https://loj.ac/p/2823)
*/
/**
 * 显然，本题可以使用字符串哈希解决
 */

using mint = OY::mint998244353;
using table_type = OY::STRHASH::SequenceHashPresumTable<mint, 128, 2000001>;
using hash_type = table_type::hash_type;
int main() {
    uint32_t n;
    std::string s;
    cin >> n;
    if (n % 2 == 0) {
        cout << "NOT POSSIBLE";
        return 0;
    }
    n /= 2;
    cin >> s;
    table_type S(s);

    // 先看前半段和后半段是否相等，如果相等的话，直接返回答案
    if (S.query(0, n - 1) == S.query(n + 1, n * 2)) {
        cout << s.substr(0, n);
        return 0;
    }

    // 假设后半部分是原字符串
    auto old = S.query(n + 1, n * 2);
    bool flag1 = false;
    for (uint32_t i = 0; i < n; i++)
        if (S.query(0, i - 1).append_right(S.query(i + 1, n)) == old) {
            flag1 = true;
            break;
        }

    // 再假设前半部分是原字符串
    old = S.query(0, n - 1);
    bool flag2 = false;
    for (uint32_t i = n; i <= n * 2; i++)
        if (S.query(n, i - 1).append_right(S.query(i + 1, n * 2)) == old) {
            flag2 = true;
            break;
        }

    if (flag1)
        if (flag2)
            cout << "NOT UNIQUE";
        else
            cout << s.substr(n + 1, n);
    else if (flag2)
        cout << s.substr(0, n);
    else
        cout << "NOT POSSIBLE";
}