#include "IO/FastIO.h"
#include "STR/SuffixArray.h"

/*
[P4051 [JSOI2007] 字符加密](https://www.luogu.com.cn/problem/P4051)
*/
/**
 * 本题求所有循环重构的字典序顺序，可以有多种做法
 * 可以使用后缀数组解决
 */

void solve_SA() {
    std::string s;
    cin >> s;

    OY::SA::SuffixArray<false, false, 200000> SA(s + s);
    for (int i = 0; i < s.size() * 2; i++) {
        int pos = SA.query_sa(i);
        if (pos < s.size()) cout << s[pos ? pos - 1 : s.size() - 1];
    }
}

int main() {
    solve_SA();
}