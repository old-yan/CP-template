### 一、模板类别

​	数据结构：树的重心，树的字典，重心分治（点分树）。

​	练习题目：

1. [P3806 【模板】点分治1](https://www.luogu.com.cn/problem/P3806)
2. [P4178 Tree](https://www.luogu.com.cn/problem/P4178)
3. [P5043 【模板】树同构（[BJOI2015]树的同构）](https://www.luogu.com.cn/problem/P5043)
4. [U104609 【模板】树的重心](https://www.luogu.com.cn/problem/U104609)
5. [#763. 树哈希](https://uoj.ac/problem/763)
6. [Rooted Tree Isomorphism Classification](https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification)(https://github.com/yosupo06/library-checker-problems/issues/796)

### 二、模板功能

​	本文件包含了三个和重心的关系较密切的模板。分别为树的重心，树的字典，以及重心分治。这三个模板均建立在树的重心的概念基础上。

#### 1.树的重心(Centroid)

1. 数据类型

   模板参数 `typename Tree` ，表示树的类型。

   构造参数 `Tree *tree` ，表示要求重心的树，无须指定根。默认为 `nullptr` 。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   本数据结构以 `reset` 方法切换要查询重心的树；以 `centroid` 方法返回重心； `max_adjacent` 方法可以查询以某个点为根时其最大子树大小。

   以某结点为根时，所有子树大小都不超过整棵树大小的一半，则称该结点为重心。

   重心的数量可能为一个或者两个，若为一个， `centroid` 返回的 `std::pair` 的第一项为重心，第二项为 `-1` ；若为两个，则第一项为编号较小的重心，第二项为编号较大的重心。

#### 2.树的字典(TreeTrie)

1. 数据类型

   输入参数 `Tree &tree` ，表示要进行字典表示的树，无须指定根。

   输入参数 `size_type root` ，表示本次进行字典表示时的临时根。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   树的哈希有各种各样的方法。本哈希法的优势为，以较快的速度实现了**确定性**的哈希，即不依赖概率，只要树结构一模一样，结点编号随便乱改都一定可以保持哈希值相同；只要树结构不一样，哈希值一定不同。

   本模板有两个静态方法可供使用，第一个静态方法需要传递树和临时根，返回一个 `std::vector` ，表示树中每个结点的子树的形态编号。第二个静态方法只需要传递树，求出以两个重心为根时，根的形态编号，组成 `std::pair` 返回。若重心只有一个，则返回值的第二项为 `-1` 。

#### 3.重心分治/点分树(CentroidDecomposition)

1. 数据类型

   模板参数 `size_type MAX_BUFFER` ，表示缓冲池的大小。一般对每棵树的结点数取最大即可。

   输入参数 `Tree &tree` ，表示要进行分治的树。

   输入参数 `PreWork &&pre_work` ，表示在分治历程中，在当前分治范围内找到分治根结点时的回调函数。

   输入参数 `Report &&report` ，表示在分治历程中，已经在当前分治范围内找到了根结点，从它向下一层分治，处理完一个下层分治时调用的回调。

   输入参数 `AfterWork &&after_work` ，表示在分治历程中，在当前分治范围内完成所有分治时，调用的回调。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   点分树，是指将重心作为树的根，进而把树分成好几部分；再在分割开的各个部分（新的树）里，继续找重心，继续进行分割，这样形成的树。显然，点分树中，上层的重心和下一层的重心之间并不一定有真实的边存在，现在连上的虚边，仅表示分治委派的关系。

   建立点分树与建立虚树具有相似性，往往只是需要在点分树/虚树的结构上进行一个 `dp` ，在构建好点分树/虚树的同时就可以完成 `dp` ，而不需要真正地把点分树/虚树建立出来。所以本模板提供了回调函数以便在构建点分树的同时进行 `dp` 转移。

   当然，如果确有真正点分树的需求，完全可以在 `report` 回调中获取到所有的边，进而构建出一棵点分树。

   本模板的常用用法为，在 `pre_work` 中，将当前分治根结点阻塞；在 `report` 中不做任何行动；在 `after_work` 中，执行在当前分治范围内的所有计算，并将当前分治根结点解除阻塞。之所以要进行阻塞，是为了实现**分割**，避免不同部分之间仍然联通。

### 三、模板示例

```c++
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
```

```
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

```