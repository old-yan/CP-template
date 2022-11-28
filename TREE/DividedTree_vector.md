### 一、模板类别

​	数据结构：分治重构树。

### 二、模板功能

 	本模板与 `DividedTree` 模板完全相同。区别仅在于，本模板适用于 `VectorTree` 。

#### 1.获取分治重构树

1. 数据类型

   输入参数 `_Tree &__tree`​ ，表示树。

   返回类型 `VectorTree` ，表示重构后的树。

2. 时间复杂度

   $O(n\cdot \log n)$ 。
   
3. 备注

   一棵任意树的高度可能是 $O(n)$ 的，而如果树高为 $O(\log n)$ ，有些暴力做法就可以说得通。通过不断找重心的方法，将原树结构替换为更加匀称的新树，从而实现了树高 $O(\log n)$ 。

   **注意：**生成的新树的边，仅表示分治委派关系，而不代表实际有边相连。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "TREE/DividedTree_vector.h"
#include <map>
#include <set>

int main() {
    // 本模板与实际题目密切相关，所以采用 https://www.luogu.com.cn/problem/P3806 解题代码作为示例代码
    int n, m;
    cin >> n >> m;
    // 建原树，注意原树不需要有根
    OY::VectorTree<int> tree(n);
    for (int i = 1; i < n; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        tree.addEdge(a - 1, b - 1, c);
    }

    // 根据原树，找出分治树的分治委派关系
    auto divide_tree = OY::getDivideTree_vector(tree);

    // 读入所有的问题
    std::map<int, std::vector<int>> questions;
    std::vector<bool> ans(m);
    for (int id = 0; id < m; id++) {
        int sum;
        cin >> sum;
        questions[sum].push_back(id);
    }
    std::vector<bool> blocked(n, false);

    // 按照分治树的分治委派关系，进行递归分治
    auto dfs = [&](auto self, int source) -> void {
        // 注意，分治的时候，source 堵住。这样可以方便限制子树范围
        blocked[source] = true;
        for (auto &adj : divide_tree.m_adj[source])
            if (int to = adj.to; !blocked[to]) self(self, to);
        blocked[source] = false;

        // 以 source 为根，进行当前层级的搜索
        std::set<int> S0{0};
        for (auto &adj : tree.m_adj[source])
            if (int to = adj.to; !blocked[to]) {
                std::set<int> S;
                int maxsum = questions.rbegin()->first;
                auto get_dis = [&](auto self, int source, int val, int from) -> void {
                    if (val <= maxsum) {
                        S.insert(val);
                        for (auto &adj : tree.m_adj[source])
                            if (int to = adj.to; !blocked[to] and to != from) self(self, to, val + adj.getDis(), source);
                    }
                };
                get_dis(get_dis, to, adj.getDis(), source);
                for (auto a : S) {
                    for (auto it = questions.begin(); it != questions.end();) {
                        auto &[sum, ids] = *it;
                        if (S0.count(sum - a)) {
                            for (int id : ids) ans[id] = true;
                            it = questions.erase(it);
                        } else
                            ++it;
                    }
                }
                if (S0.size() < S.size()) S0.swap(S);
                S0.insert(S.begin(), S.end());
            }
    };
    dfs(dfs, divide_tree.m_root);
    for (int id = 0; id < m; id++)
        cout << (ans[id] ? "AYE\n" : "NAY\n");
}
```

```
#输入
2 1
1 2 2
2

```

```
#输出如下
AYE

```


