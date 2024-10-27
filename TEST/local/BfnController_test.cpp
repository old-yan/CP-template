#include "IO/FastIO.h"
#include "TREE/BfnController.h"
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

    OY::BFN::Table<decltype(T)> bfn(&T);
    // 查看每个点的 bfs 序
    for (int i = 0; i < 9; i++) cout << bfn.m_info[i].m_bfn << " \n"[i == 8];
    // 查看所有点按 bfs 序形成的序列
    for (int i = 0; i < 9; i++) cout << bfn.m_seq[i] << " \n"[i == 8];

    // 查询  1 的 二代孩子
    bfn.do_for_descendants(1, 2, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
    // 查询  7 的 一代孩子
    bfn.do_for_descendants(7, 1, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
    // 查询  3 的 一代孩子
    bfn.do_for_descendants(3, 1, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
    // 查询  3 的 二代孩子
    bfn.do_for_descendants(3, 2, [&](int l, int r) {
        for (int i = l; i <= r; i++) cout << bfn.m_seq[i] << ",\n"[i == r];
    });
}
/*
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
4 1 5 0 2 6 7 3 8
3 1 4 7 0 2 5 6 8
8
5,6
1,4,7
0,2,5,6

*/