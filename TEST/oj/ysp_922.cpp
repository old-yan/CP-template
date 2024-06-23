#include "MATH/SqrtDecomposition.h"
#include "IO/FastIO.h"

/*
[Enumerate Quotients](https://judge.yosupo.jp/problem/enumerate_quotients)(https://github.com/yosupo06/library-checker-problems/issues/922)
*/
/**
 * 本题为数论分块模板题
 */

int main() {
    uint64_t n, cnt{};
    cin >> n;
    OY::SqrtDecomposition sd(n);
    cout << sd.size() << endl;
    for (auto x : sd) cout << x.m_quot << ' ';
}