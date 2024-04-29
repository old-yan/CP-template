#include "IO/FastIO.h"
#include "TREE/Centroid.h"
#include "TREE/FlatTree.h"

void test_centroid() {
    OY::FlatTree::Tree<bool, 1000> T(10);
    T.add_edge(0, 1);
    T.add_edge(1, 2);
    T.add_edge(2, 3);
    T.add_edge(3, 4);
    T.add_edge(4, 5);
    T.add_edge(5, 6);
    T.add_edge(6, 7);
    T.add_edge(7, 8);
    T.add_edge(8, 9);
    T.prepare();
    T.set_root(0);
    cout << T << endl;

    // 找重心
    auto centroid = OY::Centroid::Centroid<decltype(T)>(&T).centroid();
    cout << "first centroid = " << centroid.first << endl;
    cout << "second centroid = " << centroid.second << endl;

    T.resize(9);
    T.add_edge(0, 1);
    T.add_edge(1, 2);
    T.add_edge(2, 3);
    T.add_edge(3, 4);
    T.add_edge(4, 5);
    T.add_edge(5, 6);
    T.add_edge(6, 7);
    T.add_edge(7, 8);
    T.prepare();
    T.set_root(0);
    cout << T << endl;

    // 找重心
    centroid = OY::Centroid::Centroid<decltype(T)>(&T).centroid();
    cout << "first centroid = " << centroid.first << endl;
    // 此时只有一个重心
    cout << "second centroid = " << int(centroid.second) << endl;
}

void test_tree_trie() {
    OY::FlatTree::Tree<bool, 1000> T(4);
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(0, 3);
    T.prepare();
    cout << T << endl;

    // 以 0 为根时，给树中每个结点为根的子树的形态编号
    auto ids = OY::Centroid::TreeTrie::get(T, 0);
    // 显然，叶子结点形态均相同，根结点独占一种形态
    for (int i = 0; i < 4; i++) cout << "shape of " << i << " = " << ids[i] << endl;

    // 再建一棵树，把前面那棵树的形态给包含进去
    T.resize(9);
    T.add_edge(8, 0);
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(8, 3);
    T.add_edge(8, 4);
    T.add_edge(4, 5);
    T.add_edge(4, 6);
    T.add_edge(4, 7);
    T.prepare();
    cout << T << endl;

    // 以 8 为根时，给树中每个结点为根的子树的形态编号
    ids = OY::Centroid::TreeTrie::get(T, 8);
    // 我们发现，以 4 为根的子树，和前面那棵树形态完全一致
    for (int i = 0; i < 9; i++) cout << "shape of " << i << " = " << ids[i] << endl;
}

void test_centroid_decomposition() {
    // 重心分治/点分树，主要用法技巧在 oj 测试中体现，此处只建出点分树
    OY::FlatTree::Tree<bool, 1000> T(10);
    T.add_edge(0, 1);
    T.add_edge(1, 2);
    T.add_edge(2, 3);
    T.add_edge(3, 4);
    T.add_edge(4, 5);
    T.add_edge(5, 6);
    T.add_edge(6, 7);
    T.add_edge(7, 8);
    T.add_edge(8, 9);
    T.prepare();
    cout << T << endl;

    OY::FlatTree::Tree<bool, 1000> res(10);
    auto total_root = OY::Centroid::CentroidDecomposition<1000>::solve(T, {}, [&](int sub_centroid, int cur_centroid) {
        res.add_edge(sub_centroid, cur_centroid);
    },
                                                                         {});
    res.prepare(), res.set_root(total_root);
    // 显然，点分树比其原树，非常均衡
    cout << res << endl;
}

int main() {
    test_centroid();
    test_tree_trie();
    test_centroid_decomposition();
}
/*
#输出如下
[0[1[2[3[4[5[6[7[8[9]]]]]]]]]]
first centroid = 4
second centroid = 5
[0[1[2[3[4[5[6[7[8]]]]]]]]]
first centroid = 4
second centroid = -1
[0[1][2][3]]
shape of 0 = 1
shape of 1 = 0
shape of 2 = 0
shape of 3 = 0
[0[8[3][4[5][6][7]]][1][2]]
shape of 0 = 2
shape of 1 = 0
shape of 2 = 0
shape of 3 = 0
shape of 4 = 1
shape of 5 = 0
shape of 6 = 0
shape of 7 = 0
shape of 8 = 3
[0[1[2[3[4[5[6[7[8[9]]]]]]]]]]
[4[2[1[0]][3]][7[6[5]][8[9]]]]

*/