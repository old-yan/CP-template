#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDBIT.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[U335992 [提高]树上操作](https://www.luogu.com.cn/problem/U335992)
*/
/**
 * 本题要进行树上路径增值，路径和查询
 * 为树上树状数组模板题
*/
static constexpr uint32_t N = 100000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    OY::HLDBIT32<decltype(S), N> T(&S);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'P') {
            uint32_t x, y;
            cin >> x >> y;
            T.add_path<false>(x - 1, y - 1, 1);
        } else {
            uint32_t x, y;
            cin >> x >> y;
            cout << T.query_path<false>(x - 1, y - 1) << endl;
        }
    }
}