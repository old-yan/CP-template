#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[#6565. 最假女选手](https://loj.ac/p/6565)
*/
/**
 * 本题为势能线段树模板题
 */

int main() {
    uint32_t n;
    cin >> n;
    OY::ChminChmaxAddTree<int, int, int64_t, true, true, true> S(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    using node = decltype(S)::node;
    uint32_t m;
    cin >> m;
    while (m--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            int x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, node::Add{x});
        } else if (op == '2') {
            uint32_t l, r;
            int x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, node::Chmax{x});
        } else if (op == '3') {
            uint32_t l, r;
            int x;
            cin >> l >> r >> x;
            S.add(l - 1, r - 1, node::Chmin{x});
        } else if (op == '4') {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query<node::SumGetter>(l - 1, r - 1) << endl;
        } else if (op == '5') {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query<node::MaxGetter>(l - 1, r - 1) << endl;
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << S.query<node::MinGetter>(l - 1, r - 1) << endl;
        }
    }
}