#include "DS/SparseTable.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/RMQLCA.h"
#include "TREE/VirtualTree.h"

int main() {
    // 虚树主要用法技巧在 oj 测试中体现，此处只找出所有虚树的边
    OY::FlatTree::Tree<bool, 1000> T(7);
    T.add_edge(3, 1);
    T.add_edge(1, 0);
    T.add_edge(1, 2);
    T.add_edge(3, 5);
    T.add_edge(5, 4);
    T.add_edge(5, 6);
    T.prepare();
    T.set_root(3);
    cout << T << endl;

    // 假如我们希望虚树中包含 0 2 4 这三个结点
    int keys[] = {0, 2, 4};

    // 借助 RMQLCA 查询 dfs 序和 lca
    OY::RMQLCA::Table<decltype(T), OY::STMinTable<uint32_t>> rmq_lca(&T);
    std::vector<std::pair<int, int>> edges;
    OY::VTREE::VirtualTree<1000>::solve(
        keys, keys + 3, [&](int a) { return rmq_lca.m_dfn[a]; }, [&](int a, int b) { return rmq_lca.calc(a, b); }, [&](int a, int b) { edges.emplace_back(a, b); });
    cout << "in virtual tree:\n";
    for (auto &e : edges) {
        cout << e.first << " is connected with " << e.second << endl;
    }
}
/*
#输出如下
[3[1[0][2]][5[4][6]]]
in virtual tree:
0 is connected with 1
2 is connected with 1
1 is connected with 3
4 is connected with 3

*/