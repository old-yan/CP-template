#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "STR/SequenceHash.h"
#include "STR/SuffixArray.h"

/*
[P2870 [USACO07DEC] Best Cow Line G](https://www.luogu.com.cn/problem/P2870)
*/
/**
 * 本题求正向和反向的后缀子串字典序顺序，可以有多种做法
 * 可以使用后缀数组解决
 * 也可以使用字符串哈希解决
 */

void solve_SA() {
    uint32_t n;
    cin >> n;
    std::string s;
    s.reserve(n * 2 + 1);
    for (uint32_t i = 0; i < n; i++) {
        char c;
        cin >> c;
        s += c;
    }
    s += "#";
    s.append(s.rbegin() + 1, s.rend());

    OY::SA_string<1000001> SA(s);
    SA.get_rank();
    int k = 0;
    for (int l = 0, r = n - 1; l <= r;) {
        if (SA.query_rank(l) < SA.query_rank(n * 2 - r))
            cout << s[l++];
        else
            cout << s[r--];
        if (++k == 80) k = 0, cout << endl;
    }
}

void solve_hash() {
    using mint = OY::mint998244353;
    using table_type = OY::STRHASH::SequenceHashPresumTable<mint, 128, 500000>;
    using hash_type = table_type::hash_type;
    uint32_t n;
    cin >> n;
    std::string s, s2;
    s.reserve(n);
    for (uint32_t i = 0; i < n; i++) {
        char c;
        cin >> c;
        s += c;
    }
    s2.assign(s.rbegin(), s.rend());

    table_type S1(s), S2(s2);
    int k = 0;
    for (int l = 0, r = n - 1; l <= r;) {
        if (table_type::compare(S1, l, r, S2, n - 1 - r, n - 1 - l) < 0)
            cout << s[l++];
        else
            cout << s[r--];
        if (++k == 80) k = 0, cout << endl;
    }
}

int main() {
    solve_SA();
    // solve_hash();
}