#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[And or Max](https://csacademy.com/contest/round-70/task/and-or-max/statistics/)
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BitAndOrTree<uint32_t, false, true, 1 << 20> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    using node = decltype(S)::node;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.add(l - 1, r - 1, node::ChBitand{x});
        } else if (op == '2') {
            uint32_t x;
            cin >> x;
            S.add(l - 1, r - 1, node::ChBitor{x});
        } else {
            cout << S.query<node::MaxGetter>(l - 1, r - 1) << endl;
        }
    }
}
