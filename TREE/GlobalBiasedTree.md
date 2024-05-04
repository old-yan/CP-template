### 一、模板类别

​	数据结构：全局平衡二叉树（ `GBT` ）。

​	练习题目：

1. [P2056 [ZJOI2007] 捉迷藏](https://www.luogu.com.cn/problem/P2056)
2. [P2486 [SDOI2011] 染色](https://www.luogu.com.cn/problem/P2486)
3. [P3178 [HAOI2015] 树上操作](https://www.luogu.com.cn/problem/P3178)
4. [P3384 【模板】重链剖分/树链剖分](https://www.luogu.com.cn/problem/P3384)
5. [P4211 [LNOI2014] LCA](https://www.luogu.com.cn/problem/P4211)
6. [P4719 【模板】"动态 DP"&动态树分治](https://www.luogu.com.cn/problem/P4719)
7. [P4751 【模板】"动态DP"&动态树分治（加强版）](https://www.luogu.com.cn/problem/P4751)

### 二、模板功能

#### 1.构造全局平衡二叉树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `template <typename> typename NodeWrapper` ，表示树中的结点结构体模板类，需传递一个 `CRTP` 基类。

   模板参数 `bool UpdateSubtree` ，表示本模板是否要支持虚孩子树。

   模板参数 `size_type MAX_NODE` ，表示最大结点数，默认为 `1 << 20` 。

   构造参数 `size_type vertex_cnt` ，表示本树具体的结点数。默认为 `0` 。

   构造参数 `Modify &&modify` ，表示在初始化时，对每个结点调用的回调函数。默认为 `Ignore` 类对象，表示对结点不做操作。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板名为全局平衡二叉树，实则维护的是任意树。

   在初始化时，所有结点之间均为割裂状态，所以此时模板中存在 `n` 个大小为 `1` 的单结点树。

   通过 `add_edge` 方法添加边，然后通过 `prepare` 建立树。

   模板参数 `NodeWrapper` 类型须包含一个用于传递子类类型的模板参数。

   如果想支持链上信息查询，结点须满足以下要求：

   1. 实现成员函数 `pushup` ，接受左、右孩子结点的指针作为参数，更新以获取链上聚合信息。

   如果想支持链上修改，结点须满足以下要求：

   1. 实现成员函数 `pushdown` ，接受左、右孩子结点的指针作为参数，以下放修改信息。

   如果想支持子树查询，结点需满足以下要求：
   
1. 实现成员函数 `pushup` ，接受左、右、虚孩子结点的指针作为参数，更新以获取子树聚合信息。
   2. 实现成员函数 `vpushup` ，接受虚孩子树种，左、右孩子结点的指针作为参数，更新以获取虚孩子树聚合信息。

   如果想支持子树修改，结点须满足以下要求：
   
1. 实现成员函数 `pushdown` ，接受左、右、虚孩子结点的指针作为参数，以下放修改信息。
   2. 实现成员函数 `vpushdown` ，接受虚孩子树左、右结点的指针作为参数，以下放修改信息。

   显然， `GBT` 对于结点的要求并不高，因为 `GBT` 本身的操作比起结点的定义要更复杂，

   **注意：**

    `GBT` 往往和 `LCT` 同时被提及，当树结构不发生变化时，更偏向使用 `GBT` 。

   要想灵活写出各种 `GBT` 的配套结点，必须对 `GBT` 的运作的基本概念有一些了解。

   要想学习 `GBT` ，需要先了解掌握重链剖分/长链剖分的知识。 `GBT` 沿用了重链剖分中的重儿子选择标准。

   在不维护虚孩子树的 `GBT` 中，子结点永远可以找到父结点，但是父结点并非任何时候都能找到所有的儿子，它只能找到重儿子。

    `GBT` 在链的保存上也有区别。 `GBT` 把一条重链以一棵加权平衡二叉树的形态保存，加权的目的是使每个结点到根的距离不会太大。基于此，链信息由二叉树加以维护。

   基础 `GBT` 无法实现对子树信息的维护，因为父结点无法找到所有的儿子，就无法实现修改信息的下放。如果在每个结点处，再建一棵平衡二叉树以保存所有的虚孩子，就可以找到儿子们了，进而实现子树修改和子树查询。

   **注意：**

   在讨论时，原树结构是一个概念，原树经过重链剖分所得链呈现出的平衡二叉树是另一个概念，后者只对应前者的一条自上而下的链。

   **注意：**

   下文会用到一个概念叫**”维护子树“** 。显然，当 `UpdateSubtree` 为真时，本模板会维护子树；但是，即使当 `UpdateSubtree` 为假时，如果所传的结点存在添加/移除虚孩子时的回调函数 `add_vtree/remove_vtree` ，本模板虽然没有维护虚孩子树，但是同样有维护子树的功能，虽然只能起到不带修的子树查询的功能。

   **注意：**

   以下各种方法均要求结点编号从 `0` 开始。

   **注意：**

   在以下若干个 `do_for...` 方法中，本模板与 `LCT` 模板存在重大区别：需要传递多个回调函数。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示本树具体的结点数。默认为 `0` 。

   输入参数 `Modify &&modify` ，表示在初始化时，对每个结点调用的回调函数。默认为 `Ignore` 类对象，表示对结点不做操作。

2. 时间复杂度

   $O(n)$ 。

#### 3.获取某结点(get_node)

1. 数据类型

   输入参数 `size_type i` ，表示结点编号。

   返回类型 `node *` ，表示结点指针。

2. 时间复杂度

    $O(1)$ .

3. 备注

   本方法获取编号对应的结点指针，但不保证结点已经为最新。换言之，如果在本结点上方有未下放的修改量，则对本结点的信息读取会引发错误。

#### 4.获取结点编号(get_id)

1. 数据类型

   输入参数 `node *x` ，表示要查询的结点。

   返回类型 `size_type` ，表示结点的编号。

2. 时间复习度

    $O(1)$ 。

3. 备注

   本方法可以查询 `GBT` 中，某结点对应的结点编号。

#### 5.对结点调用回调(do_for_node)

1. 数据类型

   模板参数 `bool ReadOnly` ，表示是否进行只读操作。

   输入参数 `size_type a` ，表示要操作的结点的编号。

   输入参数 `Callback &&call` ，表示要对结点进行的操作函数。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   在回调函数中不需要考虑维护聚合信息，只需要读写结点自身的信息。

   如果只进行结点信息的读取，而不作修改，可将 `ReadOnly` 设为真。

#### 6.对到根的路径调用回调(do_for_path_to_root)

1. 数据类型

   模板参数 `bool ReadOnly` ，表示是否进行只读操作。默认为 `false` 。

   输入参数 `size_type a` ，表示路径的最低端的结点。

   输入参数 `SubTreeCallback &&tree_call` ，表示要对路径上进行的子树操作函数。

   输入参数 `NodeCallback &&node_call` ，表示要对路径上进行的结点操作函数。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   在回调函数中需要考虑维护聚合信息。

   由于一条路径可能由平衡二叉树的一些子树和结点拼凑而成，而 `GBT` 并不像 `LCT` 一样可以把这些碎片拼在一起接受修改，所以需要传递两种回调函数。

   如果只进行路径信息的读取，而不作修改，可将 `ReadOnly` 设为真。

#### 7.对路径调用回调(do_for_path)

1. 数据类型

   模板参数 `bool ReadOnly` ，表示是否进行只读操作。默认为 `false` 。

   模板参数 `bool DirectedCall` ，表示进行路径上的信息累积时，是否在意信息的累计方向。默认为 `false` 。

   输入参数 `size_type a` ，表示路径一端的结点。

   输入参数 `size_type b` ，表示路径另一端的结点。

   输入参数 `SubTreeCallback &&tree_call` ，表示要对路径上进行的子树操作函数。

   输入参数 `NodeCallback &&node_call` ，表示要对路径上进行的结点操作函数。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   在回调函数中需要考虑维护聚合信息。

   由于一条路径可能由平衡二叉树的一些子树和结点拼凑而成，而 `GBT` 并不像 `LCT` 一样可以把这些碎片拼在一起接受修改，所以需要传递两种回调函数。

   如果只进行路径信息的读取，而不作修改，可将 `ReadOnly` 设为真。

#### 8.对子树调用回调(do_for_subtree)

1. 数据类型

   模板参数 `bool ReadOnly` ，表示是否进行只读操作。默认为 `false` 。

   输入参数 `size_type a` ，表示要进行操作的子树的根。

   输入参数 `SubTreeCallback &&tree_call` ，表示要对子树进行的子树操作函数。

   输入参数 `NodeCallback &&node_call` ，表示要对子树进行的结点操作函数。

   输入参数 `VRootCallback &&vroot_call` ，表示要对子树进行的虚子树操作函数。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   在回调函数中需要考虑维护聚合信息。

   由于原树的一个子树可能由平衡二叉树的一些子树和结点、虚子树拼凑而成，而 `GBT` 并不像 `LCT` 一样可以把这些碎片拼在一起接受修改，所以需要传递三种回调函数。

   如果只进行路径信息的读取，而不作修改，可将 `ReadOnly` 设为真。
   
   **注意：**
   
   当 `UpdateSubtree` 为 `true` 时，自然可以利用虚孩子树实现原树的子树读写。然而，当 `UpdateSubtree` 为 `false` 时，同样也可以利用 `add_vtree` 和 `remove_vtree` 实现原树的子树信息的维护。此时，同样可以调用本方法，但是只能进行子树信息的查询；且传递的 `vroot_call` 并非对虚孩子树的根调用的回调，而是对每个结点调用的回调，用于累积该结点的虚孩子们的信息。

#### 9.查询最近公共祖先(lca)

1. 数据类型

   输入参数 `size_type a` ，表示一个结点的编号。

   输入参数 `size_type b` ，表示另一个结点的编号。

2. 时间复杂度

    $O(\log n)$ 。


#### 10.查询父结点(find_parent)

1. 数据类型

   输入参数 `size_type a` ，表示要查询的结点编号。

   返回类型 `size_type` ，表示查询到的父结点的编号编号。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   如果没有父结点，返回 `-1` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/GlobalBiasedTree.h"

// 基础的 gbt
void test_gbt() {
    cout << "gbt:\n";
    OY::GBTTree<false, 1000> S(10);
    S.add_edge(3, 5);
    S.add_edge(8, 5);
    S.add_edge(6, 7);
    S.add_edge(5, 2);
    S.add_edge(7, 2);
    S.add_edge(2, 4);
    S.add_edge(1, 4);
    S.add_edge(0, 9);
    S.add_edge(9, 5);
    S.prepare();

    // 通过 get_id 可以获取结点的编号
    for (int i = 0; i < 10; i++) cout << S.get_id(S.get_node(i)) << " \n"[i == 9];

    // 查询 lca
    cout << "lca of 5 and 6 = " << S.lca(5, 6) << endl;

    // 查询父结点
    cout << "father of 7 = " << S.find_parent(7) << endl
         << endl;
}

// 维护路径的和，支持路径增值的 lct
template <typename Node>
struct NodeWrap_link {
    int m_val, m_size_on_link, m_inc_on_link, m_sum_on_link;
    // 本函数是个辅助函数
    void add_some_value(int inc) {
        m_val += inc;
        m_inc_on_link += inc;
        m_sum_on_link += inc * m_size_on_link;
    }
    void pushup(Node *lchild, Node *rchild) {
        m_sum_on_link = m_val, m_size_on_link = 1;
        if (!lchild->is_null()) {
            m_sum_on_link += lchild->m_sum_on_link;
            m_size_on_link += lchild->m_size_on_link;
        }
        if (!rchild->is_null()) {
            m_sum_on_link += rchild->m_sum_on_link;
            m_size_on_link += rchild->m_size_on_link;
        }
    }
    void pushdown(Node *lchild, Node *rchild) {
        if (m_inc_on_link) {
            if (!lchild->is_null()) lchild->add_some_value(m_inc_on_link);
            if (!rchild->is_null()) rchild->add_some_value(m_inc_on_link);
            m_inc_on_link = 0;
        }
    }
};
void test_link() {
    cout << "gbt to maintain link:\n";
    using Tree = OY::GBT::Tree<NodeWrap_link, false, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    S.add_edge(3, 5);
    S.add_edge(8, 5);
    S.add_edge(6, 7);
    S.add_edge(5, 2);
    S.add_edge(7, 2);
    S.add_edge(2, 4);
    S.add_edge(1, 4);
    S.add_edge(0, 9);
    S.add_edge(9, 5);
    S.prepare();

    for (int i = 0; i < 10; i++) cout << S.get_node(i)->m_val << " \n"[i == 9];

    int sum = 0;
    S.do_for_path(
        5, 6, [&](node *p) { sum += p->m_sum_on_link; }, [&](node *p) { sum += p->m_val; });
    cout << "sum from 5 to 6 = " << sum << endl;

    S.do_for_path(
        5, 6, [](node *p) { p->add_some_value(10); }, [](node *p) { p->m_val += 10; });

    sum = 0;
    S.do_for_path(
        5, 6, [&](node *p) { sum += p->m_sum_on_link; }, [&](node *p) { sum += p->m_val; });
    cout << "sum from 5 to 6 = " << sum << endl
         << endl;
}

// 维护子树的和，支持子树增值的 gbt
// 表面看起来代码很长，但是这里是对结点判断是否为空之后再去加
// 其实，空结点值等于 0，所以如果不判空，直接加，代码就短多了
template <typename Node>
struct NodeWrap_subtree {
    int m_val, m_size, m_inc, m_sum;
    int m_virtual_size, m_virtual_inc, m_virtual_sum;
    // 本函数是个辅助函数
    void add_some_value(int inc) {
        m_val += inc;
        m_inc += inc;
        m_sum += inc * m_size;
    }
    void vadd_some_value(int inc) {
        m_virtual_inc += inc;
        m_virtual_sum += inc * m_virtual_size;
    }
    // 此处必须考虑虚孩子们
    void pushup(Node *lchild, Node *rchild, Node *vroot) {
        m_sum = m_val, m_size = 1;
        if (!lchild->is_null()) {
            m_sum += lchild->m_sum;
            m_size += lchild->m_size;
        }
        if (!rchild->is_null()) {
            m_sum += rchild->m_sum;
            m_size += rchild->m_size;
        }
        if (!vroot->is_null()) {
            m_sum += vroot->m_virtual_sum;
            m_size += vroot->m_virtual_size;
        }
    }
    void vpushup(Node *vlchild, Node *vrchild) {
        m_virtual_sum = m_sum, m_virtual_size = m_size;
        if (!vlchild->is_null()) {
            m_virtual_sum += vlchild->m_virtual_sum;
            m_virtual_size += vlchild->m_virtual_size;
        }
        if (!vrchild->is_null()) {
            m_virtual_sum += vrchild->m_virtual_sum;
            m_virtual_size += vrchild->m_virtual_size;
        }
    }
    void pushdown(Node *lchild, Node *rchild, Node *vroot) {
        if (m_inc) {
            if (!lchild->is_null()) lchild->add_some_value(m_inc);
            if (!rchild->is_null()) rchild->add_some_value(m_inc);
            if (!vroot->is_null()) vroot->vadd_some_value(m_inc);
            m_inc = 0;
        }
    }
    void vpushdown(Node *vlchild, Node *vrchild) {
        if (m_virtual_inc) {
            if (!vlchild->is_null()) vlchild->vadd_some_value(m_virtual_inc);
            if (!vrchild->is_null()) vrchild->vadd_some_value(m_virtual_inc);
            this->add_some_value(m_virtual_inc);
            m_virtual_inc = 0;
        }
    }
};
void test_subtree() {
    cout << "gbt to maintain subtree:\n";
    using Tree = OY::GBT::Tree<NodeWrap_subtree, true, 1000>;
    using node = Tree::node;
    // 在初始化的时候就可以做初始化
    Tree S(10, [&](node *p) {
        p->m_val = (S.get_id(p) + 1) * 100;
    });
    // 十个点，八条边，这个森林有两棵树
    S.add_edge(3, 5);
    S.add_edge(8, 5);
    S.add_edge(6, 7);
    S.add_edge(5, 2);
    S.add_edge(7, 2);
    S.add_edge(2, 4);
    S.add_edge(1, 4);
    S.add_edge(0, 9);
    S.add_edge(9, 5);
    S.set_root(4);
    S.prepare();

    int sum = 0;
    S.do_for_subtree(
        2, [&](node *p) { sum += p->m_sum; }, [&](node *p) { sum += p->m_val; }, [&](node *p) { sum += p->m_virtual_sum; });
    cout << "sum from subtree(2) = " << sum << endl;

    S.do_for_subtree(
        2, [&](node *p) { p->add_some_value(10); }, [&](node *p) { p->m_val += 10; }, [&](node *p) { p->vadd_some_value(10); });

    sum = 0;
    S.do_for_subtree(
        2, [&](node *p) { sum += p->m_sum; }, [&](node *p) { sum += p->m_val; }, [&](node *p) { sum += p->m_virtual_sum; });
    cout << "sum from subtree(2) = " << sum << endl
         << endl;
}

int main() {
    test_gbt();
    test_link();
    test_subtree();
}
```

```
#输出如下
gbt:
0 1 2 3 4 5 6 7 8 9
lca of 5 and 6 = 5
father of 7 = 2

gbt to maintain link:
100 200 300 400 500 600 700 800 900 1000
sum from 5 to 6 = 2400
sum from 5 to 6 = 2440

gbt to maintain subtree:
sum from subtree(2) = 4800
sum from subtree(2) = 4880

```

