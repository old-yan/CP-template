#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeDiameter.h"

int main() {
    // 无权树的边权视为 1
    OY::FlatTree::Tree<bool, 1000> T(9);
    T.add_edge(8, 0);
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(8, 3);
    T.add_edge(8, 4);
    T.add_edge(4, 5);
    T.add_edge(4, 6);
    T.add_edge(4, 7);
    T.prepare();
    T.set_root(0);
    cout << T << endl;

    // 找直径
    auto diameter_info = OY::TREEDIAM::solve<int>(T);
    cout << "source = " << diameter_info.m_source << endl;
    cout << "target = " << diameter_info.m_target << endl;
    cout << "length = " << diameter_info.length() << endl;

    std::vector<int> vertexes;
    diameter_info.trace([&](int i) { vertexes.push_back(i); });
    for (auto i : vertexes) cout << i << endl;
}
/*
#输出如下
[0[8[3][4[5][6][7]]][1][2]]
source = 5
target = 1
length = 4
1
0
8
4
5

*/