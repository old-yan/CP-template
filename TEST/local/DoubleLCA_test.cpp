#include "IO/FastIO.h"
#include "TREE/DoubleLCA.h"
#include "TREE/FlatTree.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(5);
    // 加边
    T.add_edge(2, 0);
    T.add_edge(1, 3);
    T.add_edge(4, 0);
    T.add_edge(0, 3);
    T.prepare();
    T.set_root(3);
    cout << T << endl;
    // LCA 预处理
    OY::DoubleLCA::Table<decltype(T), 1000, 1000 << 10> LCA(&T);
    // 查询 祖先
    cout << "father of father of 4: " << LCA.get_ancestor(4, 2) << endl;
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA.calc(2, 4) << endl;
}
/*
#输出如下
[3[1][0[2][4]]]
father of father of 4: 3
lca of 2 and 4: 0
*/