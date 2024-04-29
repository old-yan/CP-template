### 一、模板类别

​	数据结构：自定义树。

​	练习题目：

1. [剖分](https://ac.nowcoder.com/acm/problem/244121)

### 二、模板功能

​	从 `FlatTree` ， `LinkTree` 和 `VectorTree` 可以看出，一个树模板，最核心的功能，就是能够找到邻接表，这样就可以对邻接点、邻接边进行各种操作。

​	有时候，你已经把树数据以其他形式存储好了，不想再重新构建一个 `FlatTree` 等等，而是想复用数据；有时候，树的逻辑结构极其简单，即使不存储邻接表，也能根据逻辑去访问邻接表。此时可以使用自定义树。如果不理解，可以参考在 `oj` 代码中对自定义树的使用。

#### 1.构造树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `typename Tp` ，表示树中边权的类型。默认值为 `bool` ，表示边不带权值。

   构造参数 `size_type vertex_cnt` ，表示本树具体的结点数。默认为 `0` 。

   构造参数 `AdjCallback &&adj_call` ，表示要进行邻接表遍历时调用的回调。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板一经构造好，可以直接使用而无需加边、预备。

   **注意：**

   以下各种方法均要求结点编号从 `0` 开始。

#### 2.设置根(set_root)

1. 数据类型

   输入参数 `size_type root` ，表示要设置为的根结点编号。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   在调用本方法前，树的根默认为 `-1` 。

#### 3.结点数(vertex_cnt)

1. 数据类型

   返回类型 `size_type` ，表示结点数量。

2. 时间复杂度

   $O(1)$ 。

#### 4.对相邻点依次调用回调(do_for_each_adj_vertex)

1. 数据类型

   输入参数 `size_type a` ，表示当前结点。

   输入参数 `Callback &&call` ，表示要对邻接点调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指结点 `a` 的度。

3. 备注

   第二个参数 `call` 必须为可调用对象，入参为 `size_type to` ，表示结点 `a` 的邻接点。

#### 5.对相邻边依次调用回调(do_for_each_adj_edge)

1. 数据类型

   输入参数 `size_type a` ，表示当前结点。

   输入参数 `Callback &&call` ，表示要对邻接边调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指结点 `a` 的度。

3. 备注

   第二个参数 `call` 必须为可调用对象，入参为 `(size_type to, Tp dis)` ，分别表示结点 `a` 的邻接点以及之间的边长。

#### 6.基于点的树形dp(tree_dp_vertex)

1. 数据类型

   输入参数 `size_type a` ，表示进行树形 `dp` 时的起始结点，或者说根结点。

   输入参数 `PreWork &&pre_work` ，表示在 `dfs` 历程中，每当来到一个结点处时，最先调用的回调函数。

   输入参数 `Report &&report` ，表示在 `dfs` 历程中，每当来到一个结点处时，对其一个邻接点递归返回后，调用的回调函数。

   输入参数 `AfterWork &&after_work` ，表示在 `dfs` 历程中，每当来到一个结点处时，对所有邻接点递归返回后，调用的回调函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   在调用本方法之前，不需要先指定树的常态根，只须指定本次的临时根。
   
    `pre_work` 必须为可调用对象，入参为 `size_type a, size_type p` ，分别表示当前结点和父结点。出发点的父结点为 `-1` 。

   若 `pre_work` 返回类型为 `bool` 时，可以用来剪枝；若返回 `false` 则不会在当前路径下继续 `dfs` ，而是直接返回。
   
    `report` 必须为可调用对象，入参为 `size_type a, size_type to` ，分别表示当前结点和刚完成递归的邻接点。邻接点一定不为父结点。
   
    `after_work` 必须为可调用对象，入参为 `size_type a` ，表示当前结点。

#### 7.基于边的树形dp(tree_dp_edge)

1. 数据类型

   输入参数 `size_type a` ，表示进行树形 `dp` 时的起始结点，或者说根结点。

   输入参数 `PreWork &&pre_work` ，表示在 `dfs` 历程中，每当来到一个结点处时，最先调用的回调函数。

   输入参数 `Report &&report` ，表示在 `dfs` 历程中，每当来到一个结点处时，对其一个邻接点递归返回后，调用的回调函数。

   输入参数 `AfterWork &&after_work` ，表示在 `dfs` 历程中，每当来到一个结点处时，对所有邻接点递归返回后，调用的回调函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法除额外使用边的权值信息外，与 `tree_dp_vertex` 相同。

   在调用本方法之前，不需要先指定树的常态根，只须指定本次的临时根。

    `pre_work` 必须为可调用对象，入参为 `size_type a, size_type p, Tp dis` ，分别表示当前结点和父结点，及之间的边的权值。出发点的父结点为 `-1` ，之间的边的权值为 `0` 。

   若 `pre_work` 返回类型为 `bool` 时，可以用来剪枝；若返回 `false` 则不会在当前路径下继续 `dfs` ，而是直接返回。

    `report` 必须为可调用对象，入参为 `size_type a, size_type to, Tp dis` ，分别表示当前结点和刚完成递归的邻接点，及之间的边的权值。邻接点一定不为父结点。

    `after_work` 必须为可调用对象，入参为 `size_type a, Tp dis` ，表示当前结点，及与父结点间的边的权值。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/CustomTree.h"
#include "TREE/HeavyLightDecomposition.h"

struct AdjCallback {
    template <typename Callback>
    void operator()(int a, Callback &&call) const {
        if (a) call((a - 1) / 2, 1);
        if (a * 2 + 1 < 8) call(a * 2 + 1, 1);
        if (a * 2 + 2 < 8) call(a * 2 + 2, 1);
    }
};
void test_complete_binary_tree() {
    // 一个完全二叉树
#if CPP_STANDARD >= 201402L
    // 此处 call 泛指当找到 a 的邻居之后，对邻居调用的回调函数
    // 注意，call 需要支持两个入参，第一个入参为邻居，第二个入参为边权
    auto adj_call = [](int a, auto call) -> void {
        if (a) call((a - 1) / 2, 1);
        if (a * 2 + 1 < 8) call(a * 2 + 1, 1);
        if (a * 2 + 2 < 8) call(a * 2 + 2, 1);
    };
#else
    // 语言版本低的话，只能把 AdjCallback 声明在外面，而不能用匿名函数
    auto adj_call = AdjCallback();
#endif
    OY::CustomTree::Tree<bool, decltype(adj_call)> T(8, adj_call);
    // 定根
    T.set_root(0);
    // 输出观察树形(粘贴到http://mshang.ca/syntree/)
    cout << "T:" << T << endl;

    // 邻接表的遍历
    cout << "neighbors of 3:\n";
    T.do_for_each_adj_vertex(3, [&](int a) {
        cout << a << " is connected with 3\n";
    });

    // 一般情况下我们可以利用 tree_dp_vertex 快捷地进行树形 dp
    // 树形 dp 求深度（需要信息从上往下传播）
    std::vector<int> dep(8);
    T.tree_dp_vertex(0, [&](int a, int p) { dep[a] = ~p ? dep[p] + 1 : 0; }, {}, {});
    cout << "dep of tree vertexes:\n";
    for (int i = 0; i < 8; i++) cout << dep[i] << (i == 5 ? '\n' : ' ');

    // 树形 dp 求子树大小（需要信息从下往上传播）
    std::vector<int> size(8);
    T.tree_dp_vertex(
        0, [&](int a, int p) { size[a] = 1; }, [&](int a, int to) { size[a] += size[to]; }, {});
    cout << "size of tree vertexes:\n";
    for (int i = 0; i < 8; i++) cout << size[i] << (i == 5 ? '\n' : ' ');
    cout << endl;
}

void test_adapt_to_vector() {
    // 假如你特别喜欢用 std::vector<std::vector<int>> 存图，你又想用我的 hld 模板求 lca
    // 而且你还喜欢下标从 1 开始
    std::vector<std::vector<int>> V;
    int n = 7;
    V.resize(n + 1);
    auto add_edge = [&](int a, int b) {
        V[a].push_back(b);
        V[b].push_back(a);
    };
    add_edge(4, 2);
    add_edge(4, 7);
    add_edge(7, 1);
    add_edge(7, 3);
    add_edge(7, 5);
    add_edge(5, 6);

    // 好了，现在你自己的树存完了。现在，你需要写个函数找到每个点的邻居
    // 注意这个函数是要传给我的模板的，所以下标要从 0 开始，调整一下
#if CPP_STANDARD >= 201402L
    auto call_adj = [&](int a, auto call) {
        for (auto b : V[a + 1]) call(b - 1, 1);
    };
#else
    auto call_adj = [&](int a, std::function<void(int, bool)> call) {
        for (auto b : V[a + 1]) call(b - 1, 1);
    };
#endif
    OY::CustomTree::Tree<bool, decltype(call_adj)> T(n, call_adj);
    // 注意，我这里根是 3，说明在你那儿，根是 4
    T.set_root(3);
    // 可以进行输出
    cout << T << endl;
    // 对，就这么简单，也没有额外浪费空间，你的图数据的信息已经被存好了

    // 现在尝试套一下我的 lca 模板
    OY::HLD::Table<decltype(T)> hld(&T);
    // 求一下 lca(1,5) （对于你来说是 lca(2,6)）
    // 结果是 3（对于你来说是 4)
    cout << "lca(1, 5) = " << hld.calc(1, 5) << endl;
}

int main() {
    test_complete_binary_tree();
    test_adapt_to_vector();
}
```

```
#输出如下
T:[0[1[3[7]][4]][2[5][6]]]
neighbors of 3:
1 is connected with 3
7 is connected with 3
dep of tree vertexes:
0 1 1 2 2 2
2 3 size of tree vertexes:
8 4 3 2 1 1
1 1 
[3[1][6[0][2][4[5]]]]
lca(1, 5) = 3

```

