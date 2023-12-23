### 一、模板类别

​	数据结构：树（链表模式），俗称链式前向星。

### 二、模板功能

#### 1.构造树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中编号的类型。

   模板参数 `typename Tp` ，表示树中边权的类型。默认值为 `bool` ，表示边不带权值。

   模板参数 `size_type MAX_VERTEX` ，表示结点总数的上限。

   构造参数 `size_type vertex_cnt` ，表示本树具体的结点数。默认为 `0` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板和 `FlatTree` ， `VectorTree` 模板一样都是树的存储模板。与 `FlatTree` 相比，本树在加边后，邻接表就已就绪，无须预处理，启动速度快。与 `VectorTree` 相比，本树没有使用 `std::vector` 来存储邻接表，避免了动态扩容的开销。相应的，本树的缺点为，邻接表以链表形式保存，内存不连续，如果要对邻接表进行大量访问的话，速度优势会逐渐减小，乃至不如。

   显然，添加的边数必须恰比点数少一。

   **注意：**

   以下各种方法均要求结点编号从 `0` 开始。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt`​ ，表示树中的点数。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   调用本方法后，树被清空，成为新的大小的空树，等待加边。


#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示边的一端的结点编号。

   输入参数 `size_type b` ，表示边的另一端的结点编号。

   输入参数 `Tp dis` ，表示边的权值大小。默认为 `Tp` 类的默认值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在添加边时，不关注哪端在上哪端在下。
   
   在无权值树中，不需要传递第三个参数。

#### 4.预备(prepare)

1. 数据类型

2. 时间复杂度

    $O(1)$ 。

3. 备注

   对于本数据结构，实际上并无需要进行预处理的内容，只是为了和其他模板保持一致才有此接口。

#### 5.设置根(set_root)

1. 数据类型

   输入参数 `size_type root` ，表示要设置为的根结点编号。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   在调用本方法前，树的根默认为 `-1` 。

#### 5.结点数(vertex_cnt)

1. 数据类型

   返回类型 `size_type` ，表示结点数量。

2. 时间复杂度

   $O(1)$ 。

#### 6.对相邻点依次调用回调(do_for_each_adj_vertex)

1. 数据类型

   输入参数 `size_type a` ，表示当前结点。

   输入参数 `Callback &&call` ，表示要对邻接点调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指结点 `a` 的度。

3. 备注

   第二个参数 `call` 必须为可调用对象，入参为 `size_type to` ，表示结点 `a` 的邻接点。

#### 7.对相邻边依次调用回调(do_for_each_adj_edge)

1. 数据类型

   输入参数 `size_type a` ，表示当前结点。

   输入参数 `Callback &&call` ，表示要对邻接边调用的回调函数。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指结点 `a` 的度。

3. 备注

   第二个参数 `call` 必须为可调用对象，入参为 `(size_type to, Tp dis)` ，分别表示结点 `a` 的邻接点以及之间的边长。

#### 8.基于点的树形dp(tree_dp_vertex)

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

#### 9.基于边的树形dp(tree_dp_edge)

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
#include "TREE/LinkTree.h"

void test_bool_tree() {
    // 一个无权树
    OY::LinkTree::Tree<bool, 100000> T(6);
    // 加边
    T.add_edge(0, 1);
    T.add_edge(0, 2);
    T.add_edge(0, 3);
    T.add_edge(3, 4);
    T.add_edge(3, 5);
    // 预备
    T.prepare();
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
    std::vector<int> dep(6);
    T.tree_dp_vertex(0, [&](int a, int p) { dep[a] = ~p ? dep[p] + 1 : 0; }, {}, {});
    cout << "dep of tree vertexes:\n";
    for (int i = 0; i < 6; i++) cout << dep[i] << (i == 5 ? '\n' : ' ');

    // 树形 dp 求子树大小（需要信息从下往上传播）
    std::vector<int> size(6);
    T.tree_dp_vertex(
        0, [&](int a, int p) { size[a] = 1; }, [&](int a, int to) { size[a] += size[to]; }, {});
    cout << "size of tree vertexes:\n";
    for (int i = 0; i < 6; i++) cout << size[i] << (i == 5 ? '\n' : ' ');
}

void test_int_tree() {
    // 一个边权为 int 类型的树
    OY::LinkTree::Tree<int, 100000> T(6);
    // 加边
    T.add_edge(0, 1, 100);
    T.add_edge(0, 2, 200);
    T.add_edge(0, 3, 50);
    T.add_edge(3, 4, 110);
    T.add_edge(3, 5, 80);
    // 预备
    T.prepare();
    // 定根
    T.set_root(0);
    // 输出观察树形(粘贴到http://mshang.ca/syntree/)
    cout << "T:" << T << endl;

    // 邻接表的遍历
    cout << "neighbors of 3:\n";
    // 此时我们用 do_for_each_adj_edge 来遍历邻接表，不仅可以遍历邻接点，还能知道边权
    // 当然如果你不需要边权信息，还想用 do_for_each_adj_vertex 也是可以的
    T.do_for_each_adj_edge(3, [&](int a, int dis) {
        cout << a << " is connected with 3, distance = " << dis << endl;
    });

    // tree_dp_vertex 仍然可用
    // 这里展示使用 tree_dp_edge 来求到某点的距离
    std::vector<int> distance(6);
    T.tree_dp_edge(0, [&](int a, int p, int dis) { distance[a] = ~p ? distance[p] + dis : 0; }, {}, {});
    cout << "distance of tree vertexes:\n";
    for (int i = 0; i < 6; i++) cout << distance[i] << (i == 5 ? '\n' : ' ');
}

int main() {
    test_bool_tree();
    test_int_tree();
}
```

```
#输出如下
T:[0[3[5][4]][2][1]]
neighbors of 3:
5 is connected with 3
4 is connected with 3
0 is connected with 3
dep of tree vertexes:
0 1 1 1 2 2
size of tree vertexes:
6 1 1 3 1 1
T:[0[3[5][4]][2][1]]
neighbors of 3:
5 is connected with 3, distance = 80
4 is connected with 3, distance = 110
0 is connected with 3, distance = 50
distance of tree vertexes:
0 100 200 50 160 130

```

