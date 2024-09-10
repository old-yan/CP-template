#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[Range Chmin Chmax Add Range Sum](https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/243)
*/
/**
 * 本题为势能线段树模板题
 */

int main() {
    uint32_t n, q;
    cin >> n >> q;
    auto read = [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    };
    OY::ChminChmaxAddTree<int64_t, int32_t, int64_t, true, true, true> S(n, read);
    using node = decltype(S)::node;
    for (uint32_t i = 0; i != q; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            S.add(l, r - 1, node::Chmin{b});
        } else if (op == '1') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            S.add(l, r - 1, node::Chmax{b});
        } else if (op == '2') {
            uint32_t l, r;
            int64_t b;
            cin >> l >> r >> b;
            S.add(l, r - 1, node::Add{b});
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query<node::SumGetter>(l, r - 1) << '\n';
        }
    }
}