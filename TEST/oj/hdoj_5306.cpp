#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[Gorgeous Sequence](http://acm.hdu.edu.cn/showproblem.php?pid=5306)
*/
using Tree = OY::ChminChmaxAddTree<int32_t, int32_t, int64_t, true, false, false, 4000000>;
using node = Tree::node;
int main() {
    uint32_t t = 1;
    cin >> t;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        Tree S(n, [&](uint32_t i) -> uint32_t {
            uint32_t x;
            cin >> x;
            return x;
        });
        for (uint32_t i = 0; i < m; i++) {
            char op;
            cin >> op;
            if (op == '0') {
                uint32_t l, r;
                int x;
                cin >> l >> r >> x;
                S.add(l - 1, r - 1, node::Chmin{x});
            } else if (op == '1') {
                uint32_t l, r;
                cin >> l >> r;
                auto res = S.query<node::MaxGetter>(l - 1, r - 1);
                cout << res << endl;
            } else {
                uint32_t l, r;
                cin >> l >> r;
                auto res = S.query<node::SumGetter>(l - 1, r - 1);
                cout << res << endl;
            }
        }
    }
}