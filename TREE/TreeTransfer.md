### 一、模板类别

​	数据结构：换根 `dp` 。

​	练习题目：

1. [3241. 标记所有节点需要的时间](https://leetcode.cn/problems/time-taken-to-mark-all-nodes/)
2. [P2986 [USACO10MAR] Great Cow Gathering G](https://www.luogu.com.cn/problem/P2986)
3. [P3304 [SDOI2013] 直径](https://www.luogu.com.cn/problem/P3304)
4. [P3478 [POI2008] STA-Station](https://www.luogu.com.cn/problem/P3478)
5. [P10842 【MX-J2-T3】Piggy and Trees](https://www.luogu.com.cn/problem/P10842)
6. [Tree Path Composite Sum](https://judge.yosupo.jp/problem/tree_path_composite_sum)(https://github.com/yosupo06/library-checker-problems/issues/861)


### 二、模板功能

​		本模板仅提供一个 `solve` 函数，模板参数 `typename Tp` 表示动态规划的值类型。依次传入树对象、 `InitMapping mapping` 函数、 `Merge merge` 函数、 `Exclude exclude ` 函数用于辅助状态转移。

​		在整个动态规划过程中，会进行两次 `dfs` 。

​		第一次 `dfs` 初始化每个位置的值，并完成从叶到根的信息聚合；此时每个结点的值就包含了对以其为根的子树的统计信息。在第一次 `dfs` 过程中，会使用 `mapping` 函数进行每个点的值得初始化，并使用 `merge` 函数将每个点的值向父结点进行聚合。

​		第二次 `dfs` 则是一次单纯的树的遍历，每次从父结点 `p` 走到相应的子结点 `a` 时，已经预先保证父结点 `p` 的信息包含了整棵树的信息。然后我们将 `p` 结点的信息减去 `a` 向 `p` 提供的贡献，把这个值聚合到结点 `a` 的信息里。此时就可以让 `a` 结点的信息也包含整棵树的信息。在这个过程中，需要调用 `exclude` 函数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"
#include "TREE/TreeTransfer.h"

void test_diameter() {
    // 众所周知，求直径长度需要两次 dfs，可以通过换根 dp 完成
    // 我们来换个问题：求经过每个点的最长简单路径
    OY::FlatTree::Tree<int, 1000> S(7);
    S.add_edge(0, 4, 10);
    S.add_edge(0, 5, 6);
    S.add_edge(1, 4, 8);
    S.add_edge(2, 3, 9);
    S.add_edge(2, 4, 8);
    S.add_edge(2, 6, 2);
    S.prepare(), S.set_root(0);

    // 维护每个点出发的最长的两条射线
    // m_val[0] 最长，m_val[1] 次长，且两条射线没有公共边
    struct Top2 {
        int m_val[2];
    };
    // 初始化时，每个结点都不包含任何射线
    auto mapping = [](uint32_t i) -> Top2 { return {}; };
    // 每个子结点可以给父结点提供一个射线
    auto merge = [](Top2 &dp_a, Top2 dp_son, uint32_t a, uint32_t son, int edge_value) {
        int offer = dp_son.m_val[0] + edge_value;
        if (offer > dp_a.m_val[0])
            dp_a.m_val[1] = dp_a.m_val[0], dp_a.m_val[0] = offer;
        else if (offer > dp_a.m_val[1])
            dp_a.m_val[1] = offer;
    };
    // 从父结点向子结点换根时，首先把子结点对父结点的贡献剥离掉
    // 然后再把剩余部分赋给子结点
    auto exclude = [](Top2 &dp_a, Top2 dp_fa, uint32_t a, uint32_t fa, int edge_value) {
        // 注意，如果 fa 是 -1，那就是根结点，不需要换根
        if (~fa) {
            int offer = dp_a.m_val[0] + edge_value;
            int rest = offer == dp_fa.m_val[0] ? dp_fa.m_val[1] : dp_fa.m_val[0];
            rest += edge_value;
            if (rest > dp_a.m_val[0])
                dp_a.m_val[1] = dp_a.m_val[0], dp_a.m_val[0] = rest;
            else if (rest > dp_a.m_val[1])
                dp_a.m_val[1] = rest;
        }
    };
    auto dp = OY::TreeTransfer::solve<Top2>(S, mapping, merge, exclude);
    for (int i = 0; i < S.vertex_cnt(); i++)
        cout << "longest simple path covering " << i << ": " << dp[i].m_val[0] + dp[i].m_val[1] << endl;
}

int main() {
    test_diameter();
}
```

```
#输出如下
longest simple path covering 0: 33
longest simple path covering 1: 25
longest simple path covering 2: 33
longest simple path covering 3: 33
longest simple path covering 4: 33
longest simple path covering 5: 33
longest simple path covering 6: 26

```