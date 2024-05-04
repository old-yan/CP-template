#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"

/*
[Associative Array](https://judge.yosupo.jp/problem/associative_array)(https://github.com/yosupo06/library-checker-problems/issues/376)
*/
/**
 * 本题为哈希表模板题
 */

OY::GHASH::UnorderedMap<uint64_t, uint64_t, false, 1500007> GS;
int main() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) {
        char op;
        uint64_t k;
        cin >> op >> k;
        if (op == '0') {
            uint64_t v;
            cin >> v;
            GS.insert_or_assign(k, v);
        } else
            cout << GS.get(k, 0) << endl;
    }
}