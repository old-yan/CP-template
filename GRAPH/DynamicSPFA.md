### 一、模板类别

​	数据结构：动态 `SPFA` 算法。

​	练习题目：

1. [Shortest Path](https://acm.hdu.edu.cn/showproblem.php?pid=3631)
2. [2642. 设计可以求最短路径的图类](https://leetcode.cn/problems/design-graph-with-shortest-path-calculator/)
3. [P2387 [NOI2014] 魔法森林](https://www.luogu.com.cn/problem/P2387)
4. [P2502 [HAOI2006] 旅行](https://www.luogu.com.cn/problem/P2502)
5. [P2683 小岛](https://www.luogu.com.cn/problem/P2683)
6. [P7516 [省选联考 2021 A/B 卷] 图函数](https://www.luogu.com.cn/problem/P7516)

### 二、模板功能

​		本模板与 `Dynamic Floyd` 模板功能类似，用于维护动态追加的最短路。

​		本模板提供了一个 `Solver` 和一个 `Solvers` ，前者用于处理单源最短路或者多源最短路；后者用于处理全源最短路。

​		 `Solver` 在 `run` 或者 `update_by_edge` 之前，需要手动调用 `set_distance` 设置源点的距离。

​		 `Solvers` 不需要手动调用 `set_distance` 设置源点距离，而会自动将所有结点到自己的距离设为 `group::identity()` ，作为全源最短路的起点。

### 三、模板示例

```c++
#include "GRAPH/DynamicSPFA.h"
#include "IO/FastIO.h"

void test_run() {
    // 全源最短路
    OY::VectorDynamicSPFAs<OY::DSPFA::AddGroup<int>> G(4);
    // 一次性加入很多边，然后 run 一次
    G.add_edge(0, 1, 100);
    G.add_edge(1, 0, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(1, 3, 300);
    G.add_edge(1, 2, 10);
    G.add_edge(2, 3, 50);

    G.run();
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) cout << (G.query(i, j) == G.infinite() ? -1 : G.query(i, j)) << " \n"[j == 3];
    cout << endl;
}

void test_update_by_edge() {
    // 全源最短路
    OY::VectorDynamicSPFAs<OY::DSPFA::AddGroup<int>> G(4);
    // 也可以每次加入一条边，就立刻更新
    G.update_by_edge(0, 1, 100);
    G.update_by_edge(1, 0, 100);
    G.update_by_edge(1, 2, 200);
    G.update_by_edge(1, 3, 300);
    G.update_by_edge(1, 2, 10);
    G.update_by_edge(2, 3, 50);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) cout << (G.query(i, j) == G.infinite() ? -1 : G.query(i, j)) << " \n"[j == 3];
    cout << endl;
}

void test3() {
    // 单源或多源最短路
    OY::VectorDynamicSPFA<OY::DSPFA::AddGroup<int>> G(4);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 0, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(1, 3, 300);
    G.add_edge(1, 2, 10);
    G.add_edge(2, 3, 50);

    // 设置两个源
    G.set_distance(0, 0);
    G.set_distance(2, 0);
    G.run();

    for (int j = 0; j < 4; j++) cout << (G.query(j) == G.infinite() ? -1 : G.query(j)) << " \n"[j == 3];
    cout << endl;
}

int main() {
    test_run();
    test_update_by_edge();
    test3();
}
```

```
#输出如下
0 100 110 160
100 0 10 60
-1 -1 0 50
-1 -1 -1 0

0 100 110 160
100 0 10 60
-1 -1 0 50
-1 -1 -1 0

0 100 0 50

```

