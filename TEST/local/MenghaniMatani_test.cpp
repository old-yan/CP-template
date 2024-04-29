#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/MenghaniMatani.h"

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
    OY::MenghaniMatani::Table<decltype(T)> LCA(&T);
    // 查询 祖先
    cout << "father of father of 4: " << LCA.get_ancestor(4, 2) << endl;
    // 查询父结点
    cout << "father of 4: " << LCA.find_parent(4) << endl;
    // 查询子结点
    cout << "son of 3(in the direction of 4): " << LCA.find_son(3, 4) << endl;
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA.calc(2, 4) << endl;
}
/*
#输出如下
[3[1][0[2][4]]]
father of father of 4: 3
father of 4: 0
son of 3(in the direction of 4): 0
lca of 2 and 4: 0

*/