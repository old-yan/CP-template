#include "DS/IntegerSet.h"
#include "IO/FastIO.h"

/*
[Predecessor Problem](https://judge.yosupo.jp/problem/predecessor_problem)(https://github.com/yosupo06/library-checker-problems/issues/632)
*/
/**
 * 本题为有序整数集模板题，可以使用 IntegerSet 解决
 */

OY::INTSET::Table<24> GT;
int main() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) {
        char c;
        cin >> c;
        if (c == '1') GT.set(i);
    }
    for (uint32_t i = 0; i != q; i++) {
        char op;
        uint32_t k;
        cin >> op >> k;
        if (op == '0')
            GT.set(k);
        else if (op == '1')
            GT.reset(k);
        else if (op == '2')
            cout << GT.at(k) << endl;
        else if (op == '3')
            cout << int(GT.lower_bound(k)) << endl;
        else
            cout << int(GT.smaller_bound(k + 1)) << endl;
    }
}