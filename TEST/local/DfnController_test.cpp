#include "IO/FastIO.h"
#include "TREE/DfnController.h"
#include "TREE/FlatTree.h"

int main() {
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(9);
    // 加边
    T.add_edge(3, 1);
    T.add_edge(1, 0);
    T.add_edge(1, 2);
    T.add_edge(2, 8);
    T.add_edge(3, 4);
    T.add_edge(3, 7);
    T.add_edge(7, 5);
    T.add_edge(7, 6);
    T.prepare();
    T.set_root(3);
    cout << T << endl;

    OY::DFN::Table<decltype(T)> dfn(&T);
    // 查看每个点的欧拉序
    for (int i = 0; i < 9; i++) cout << dfn.m_info[i].m_dfn << " \n"[i == 8];
    // 查看所有点按欧拉序形成的序列
    for (int i = 0; i < 9; i++) cout << dfn.m_seq[i] << " \n"[i == 8];

    std::pair<int, int> subtree_range;
    dfn.do_for_subtree(7, [&](int l, int r) { subtree_range = {l, r}; });
    cout << "subtree(7):\n";
    for (int i = subtree_range.first; i <= subtree_range.second; i++) {
        cout << dfn.m_seq[i] << " ";
    }
}
/*
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
2 1 3 0 5 7 8 6 4
3 1 0 2 8 4 7 5 6
subtree(7):
7 5 6 

*/