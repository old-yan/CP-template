#include "DS/MaskRMQ.h"
#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"

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
    OY::RMQLCA::Table<decltype(T), OY::STMinTable<uint32_t>> LCA(&T);
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA.calc(2, 4) << endl;
    cout << "lca of 1 and 4: " << LCA.calc(1, 4) << endl;

    // 换个 MaskRMQ
    OY::RMQLCA::Table<decltype(T), OY::MaskRMQMinValueTable<uint32_t>> LCA2(&T);
    // 查询 LCA
    cout << "lca of 2 and 4: " << LCA2.calc(2, 4) << endl;
    cout << "lca of 1 and 4: " << LCA2.calc(1, 4) << endl;
}
/*
#输出如下
[3[1][0[2][4]]]
lca of 2 and 4: 0
lca of 1 and 4: 3
lca of 2 and 4: 0
lca of 1 and 4: 3

*/