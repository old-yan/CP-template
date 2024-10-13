#include "IO/FastIO.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/FlatTree.h"

// 手动模式
void Ad_manual() {
    // 树可以是 FlatTree LinkTree 或者 VectorTree 均可
    // 一个无权树
    OY::FlatTree::Tree<bool, 1000> T(6);
    // 加边
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(0, 3);
    T.add_edge(3, 4);
    T.add_edge(3, 5);
    // 预备
    T.prepare();
    T.set_root(0);
    cout << T << endl;

    // 假定每个点的初值都是编号 * 1000
    OY::AdjSumTreeTable<int, decltype(T), false> Ad(&T, [&](int i) {
        return i * 1000;
    });
    cout << Ad << endl;

    // 子树增值
    Ad.switch_to_difference_downward();
    Ad.add_subtree(3, 1);
    Ad.switch_to_value();
    cout << Ad << endl;

    // 路径增值
    Ad.switch_to_difference_upward();
    Ad.add_path(1, 4, 0, -1, 10);
    Ad.switch_to_value();
    cout << Ad << endl;

    // 查询子树和
    Ad.switch_to_presum_upward();
    cout << "sum of subtree(3) = " << Ad.query_subtree(3) << endl;

    // 查询路径和
    Ad.switch_to_presum_downward();
    cout << "sum of path(1~4) = " << Ad.query_path(1, 4, 0, -1) << endl;
}

// 自动模式
void Ad_auto() {
    OY::FlatTree::Tree<bool, 1000> T(6);
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(0, 3);
    T.add_edge(3, 4);
    T.add_edge(3, 5);
    T.prepare();
    T.set_root(0);
    cout << T << endl;

    OY::AdjSumTreeTable<int, decltype(T), true> Ad(&T, [&](int i) {
        return i * 1000;
    });
    cout << Ad << endl;

    // 子树增值
    Ad.add_subtree(3, 1);
    cout << Ad << endl;

    // 路径增值
    Ad.add_path(1, 4, 0, -1, 10);
    cout << Ad << endl;

    // 查询子树和
    cout << "sum of subtree(3) = " << Ad.query_subtree(3) << endl;

    // 查询路径和
    cout << "sum of path(1~4) = " << Ad.query_path(1, 4, 0, -1) << endl;
}

int main() {
    Ad_manual();
    Ad_auto();
}
/*
#输出如下
[0[1][2][3[4][5]]]
[0[1000][2000][3000[4000][5000]]]
[0[1000][2000][3001[4001][5001]]]
[10[1010][2000][3011[4011][5001]]]
sum of subtree(3) = 12023
sum of path(1~4) = 8042
[0[1][2][3[4][5]]]
[0[1000][2000][3000[4000][5000]]]
[0[1000][2000][3001[4001][5001]]]
[10[1010][2000][3011[4011][5001]]]
sum of subtree(3) = 12023
sum of path(1~4) = 8042

*/