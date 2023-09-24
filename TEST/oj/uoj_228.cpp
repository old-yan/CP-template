#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[#228. 基础数据结构练习题](https://uoj.ac/problem/228)
*/
using Tree = OY::ChsqrtAddTree<uint64_t, uint32_t, uint64_t, 1 << 20>;
using node = Tree::node;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    Tree S(n, [](auto...) {
        uint64_t x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            S.add(l - 1, r - 1, node::Add{x});
        } else if (op == '2')
            S.add(l - 1, r - 1, node::Chsqrt{});
        else
            cout << S.query<node::SumGetter>(l - 1, r - 1) << endl;
    }
}