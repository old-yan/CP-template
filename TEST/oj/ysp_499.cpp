#include "DS/BiTrie.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[Set Xor-Min](https://judge.yosupo.jp/problem/set_xor_min)(https://github.com/yosupo06/library-checker-problems/issues/499)
*/
/**
 * 本题为 01 字典树模板题，需要实现删除功能
 */

static constexpr uint32_t Q = 500000;
int main() {
    uint32_t q;
    cin >> q;
    OY::BiTrie::Tree<uint32_t, 30, OY::BiTrie::Ignore, OY::StaticBufferWrapWithCollect<6100000>::type> S;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        uint32_t x;
        cin >> op >> x;
        if (op == '0') {
            S.insert(x);
        } else if (op == '1')
            S.erase(x);
        else
            cout << S.min_bitxor(x).second << endl;
    }
}