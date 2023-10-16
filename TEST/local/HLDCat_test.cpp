#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/HLDCat.h"

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

    // 不妨让每个结点初始值为编号的一万倍
    auto cat = OY::make_HLDCat<int, 1000, 1000 * 10>(&T, std::plus<int>(), [](int i) { return i * 10000; });
    cout << cat << endl;

    // 输出 5~8 之间的路径和
    cout << cat.query_path<true>(5, 8) << endl;
}
/*
#输出如下
[3[1[0][2[8]]][4][7[5][6]]]
[30000[10000[0][20000[80000]]][40000][70000[50000][60000]]]
260000

*/