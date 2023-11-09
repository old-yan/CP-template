#include "DS/Bitset.h"
#include "IO/FastIO.h"

/*
[P5057 [CQOI2006] 简单题](https://www.luogu.com.cn/problem/P5057)
*/
/**
 * 本题为 Bitset 模板题
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::Bitset::Table<100000> S;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            cin >> l >> r;
            S.flip(l - 1, r - 1);
        } else {
            uint32_t i;
            cin >> i;
            cout << S[i - 1] << endl;
        }
    }
}