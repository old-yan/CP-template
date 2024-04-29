#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HeavyLightDecomposition.h"

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

    OY::HLD::Table<decltype(T)> hld(&T);
    // 查看每个点的 dfs 序
    for (int i = 0; i < 9; i++) cout << hld.m_info[i].m_dfn << " \n"[i == 8];
    // 查看所有点按 dfs 序形成的序列
    for (int i = 0; i < 9; i++) cout << hld.m_seq[i] << " \n"[i == 8];

    // 查看 5 和 8 之间的路径所占的区间
    std::vector<std::pair<int, int>> ranges;
    hld.do_for_path<true>(5, 8, [&](int l, int r) {
        ranges.emplace_back(l, r);
    });
    cout << "from 5 to 8:\n";
    // 观察可以看到，找到的区间就是 5 到 8 的结点们
    for (auto &range : ranges) {
        for (int i = range.first; i <= range.second; i++) {
            cout << hld.m_seq[i] << " ";
        }
    }
    cout << endl;

    std::pair<int, int> subtree_range;
    hld.do_for_subtree(7, [&](int l, int r) { subtree_range = {l, r}; });
    cout << "subtree(7):\n";
    for (int i = subtree_range.first; i <= subtree_range.second; i++) {
        cout << hld.m_seq[i] << " ";
    }
}
/*
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
4 1 2 0 5 7 8 6 3
3 1 2 8 0 4 7 5 6
from 5 to 8:
7 5 3 1 2 8 
subtree(7):
7 5 6 

*/