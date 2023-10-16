#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDBIT.h"
#include "TREE/LinkTree.h"
#include "TREE/VectorTree.h"

/*
[P3178 [HAOI2015] 树上操作](https://www.luogu.com.cn/problem/P3178)
*/
/**
 * 本题要进行树上单点增值，子树增值，路径和查询
 * 显然为树上树状数组模板题
*/
static constexpr uint32_t N = 100000;
int64_t val[N];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) cin >> val[i];
    OY::LinkTree::Tree<bool, N> S(n);
    // OY::FlatTree::Tree<bool, N> S(n);
    // OY::VectorTree::Tree<bool> S(n);
    for (uint32_t i = 1; i < n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);

    OY::HLDBIT64<decltype(S), N> T(&S, [](uint32_t i) { return val[i]; });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            int a;
            cin >> x >> a;
            T.add(x - 1, a);
        } else if (op == '2') {
            uint32_t x;
            int a;
            cin >> x >> a;
            T.add_subtree(x - 1, a);
        } else {
            uint32_t x;
            cin >> x;
            cout << T.query_path<true>(x - 1, 0) << endl;
        }
    }
}