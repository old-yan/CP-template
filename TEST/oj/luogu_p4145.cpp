#include "DS/SegmentBeat.h"
#include "IO/FastIO.h"

/*
[P4145 上帝造题的七分钟 2 / 花神游历各国](https://www.luogu.com.cn/problem/P4145)
*/
using Tree = OY::ChsqrtAddTree<int64_t, int32_t, int64_t, 1 << 18>;
using node = Tree::node;
int main() {
    uint32_t n;
    cin >> n;
    Tree S(n, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    uint32_t m;
    cin >> m;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (l > r) std::swap(l, r);
        if (op == '0')
            S.add(l - 1, r - 1, node::Chsqrt{});
        else
            cout << S.query<node::SumGetter>(l - 1, r - 1) << endl;
    }
}