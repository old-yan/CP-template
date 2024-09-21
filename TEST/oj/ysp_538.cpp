#include "DS/MonoAVL.h"
#include "IO/FastIO.h"

/*
[Range Reverse Range Sum](https://judge.yosupo.jp/problem/range_reverse_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/538)
*/
/**
 * 本题为平衡树模板题
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    using Tree = OY::VectorMonoSumAVL<uint32_t, uint64_t, 0, true>;
    Tree::_reserve(n + 1);
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    Tree S = Tree::from_mapping(n, read);
    while (m--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '0')
            S.reverse(l, r - 1);
        else
            cout << (l == r ? 0 : S.query(l, r - 1)) << endl;
    }
}
