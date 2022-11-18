

### 一、模板类别

​	数据结构：虚树。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tree` ，表示树类型。

   构造参数 `_Tree __tree` ，表示原树。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   虚树是为压缩树结构而诞生。

   假设有一个问题，问将树结构里的带权边砍掉，使得从根结点无法到达任何的 **关键点** ，问砍掉的最小边权。这个问题只需要进行一次树上 `dp` 即可解决，时间复杂度为 $O(n)$ 。

   然而，如果题目给出的是 `100000` 次单独的询问，每次给出寥寥几个关键点。显然我们不能进行 `100000` 次一模一样的树上 `dp` 过程，否则耗时将极为巨大。

   经过分析，我们可以将树结构进行压缩，将无关紧要的 **非关键点** 忽略掉，只剩下 **关键点** 和联结多个关键组分的交叉点上的 **非关键点** 。这个树的大小和关键点的点数成正比，在这个树上进行 `dp` ，显然快得多。

   这样类似的问题，都可以利用虚树来解决。

#### 2.获取虚树边

1. 数据类型

   输入参数 `const std::vector<uint32_t>& __keyPoints`​ ，表示所有的关键点。

   返回类型 `std::vector<_VirtualEdge>` ，表示所有的虚边，也就是虚树中要用到的边。

2. 时间复杂度

   $O(n)$ ，此处 `n` 为关键点的数量。

3. 备注

   虚树中的点并非全都是关键点，也有非关键点。但是虚树中的每颗子树一定含有关键点。

   **注意：** 
   
   1. 即使 `__keyPoints` 中不含有原树根结点，习惯上会在虚树中加入原树的根结点，并作为新建立的虚树的根，也就是树的方向不会变。
   2. `__keyPoints` 中，至少需要含有一个非原树根结点的结点。


#### 3.填充虚树

1. 数据类型

   输入参数 `Tree<_MAXN, bool> &__tree` ，表示待填充的虚树。

   输入参数 `Discretizer<uint32_t> &__dis` ，表示离散化数组。

   输入参数 `const std::vector<uint32_t>& __keyPoints` ，表示所有的关键点。

   返回类型 `void` ，所有的输出信息均通过引用参数输出。

2. 时间复杂度

   $O(n)$ ，此处 `n` 为关键点的数量。

3. 备注

   本函数会先调用获取虚边的函数，然后将所有涉及到的点离散化，按照离散化后的数字构造虚树。

   结点原编号和现编号的映射关系，保存在 `__dis` 中输出。

### 三、模板示例

```c++
#include "DS/STtable.h"
#include "IO/FastIO.h"
#include "TREE/VirtualTree.h"

static constexpr int MAXN = 250000;
int main() {
    // 本模板与实际题目密切相关，所以采用 https://www.luogu.com.cn/problem/P2495 解题代码作为示例代码
    // 读入树
    int n;
    cin >> n;
    OY::Tree<MAXN, int> Tree(n);
    for (int i = 1; i <= n - 1; i++) {
        int u, v, dis;
        cin >> u >> v >> dis;
        Tree.addEdge(u - 1, v - 1, dis);
    }
    Tree.prepare();
    Tree.setRoot(0);

    // 虚树预处理
    OY::VirtualTreeFactory factory(Tree);
    OY::Tree<MAXN> virtual_tree;
    OY::Discretizer<uint32_t> dis;
    std::bitset<MAXN> iskey;
    // 注意，本模板内，包含了一个重链剖分模板
    OY::HeavyLightDecomposition<OY::Tree<MAXN, int>> &hld = factory.m_hld;

    // 取出树的 dfs 序列，将每个点到父结点的边长送到 ST 表中，方便查询虚边边长
    auto sequence = hld.getSequence();
    std::vector<int> up_edge_length(n);
    for (int i = 0; i < n; i++) {
        int cur = sequence[i];
        up_edge_length[i] = cur ? Tree.m_distances[Tree.m_starts[cur]] : INT_MAX;
    }
    OY::STTable st(up_edge_length.begin(), up_edge_length.end(), std::min);

    int q;
    cin >> q;
    while (q--) {
        int k;
        cin >> k;
        std::vector<uint32_t> keyPoints(k);
        for (int i = 0; i < k; i++) {
            int key;
            cin >> key;
            keyPoints[i] = key - 1;
            iskey.set(key - 1);
        }
        factory.fillVirtualTree(virtual_tree, dis, keyPoints);

        // 进行一个从叶到根的树上 dp
        auto mymap = [&](int i) -> int64_t { return 0; };
        auto mymerge = [&](int64_t &x, int64_t y, int i, int to) {
            int min_cost = INT_MAX;
            for (auto [l, r] : hld.getLinks(dis[i], dis[to])) min_cost = std::min(min_cost, st.query(l, r));
            if (iskey[dis[to]])
                x += min_cost;
            else
                x += std::min<int64_t>(min_cost, y);
        };
        auto dp = virtual_tree.getSubtreeValues(mymap, mymerge);
        cout << dp[0] << '\n';

        for (auto key : keyPoints) iskey.reset(key);
    }
}
```

```
#输入
10
1 5 13
1 9 6
2 1 19
2 4 8
2 3 91
5 6 8
7 5 4
7 8 31
10 7 9
3
2 10 6
4 5 7 8 3
3 9 4 6

```



```
#输出如下
12
32
22

```

