### 一、模板类别

​	数据结构：`Dijkstra` 算法。

​	练习题目：

1. [find the safest road](https://acm.hdu.edu.cn/showproblem.php?pid=1596)
2. [最短路径问题](https://acm.hdu.edu.cn/showproblem.php?pid=3790)
3. [Deleting Edges](https://acm.hdu.edu.cn/showproblem.php?pid=6026)
4. [1368. 使网格图至少有一条有效路径的最小代价](https://leetcode.cn/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/)
5. [1631. 最小体力消耗路径](https://leetcode.cn/problems/path-with-minimum-effort/)
6. [2290. 到达角落需要移除障碍物的最小数目](https://leetcode.cn/problems/minimum-obstacle-removal-to-reach-corner/)
7. [3123. 最短路径中的边](https://leetcode.cn/problems/find-edges-in-shortest-paths/)
8. [P1491 集合位置](https://www.luogu.com.cn/problem/P1491)
9. [P1576 最小花费](https://www.luogu.com.cn/problem/P1576)
10. [P1608 路径统计](https://www.luogu.com.cn/problem/P1608)
11. [P2047 [NOI2007] 社交网络](https://www.luogu.com.cn/problem/P2047)
12. [P2984 [USACO10FEB] Chocolate Giving S](https://www.luogu.com.cn/problem/P2984)
13. [P3371 【模板】单源最短路径（弱化版）](https://www.luogu.com.cn/problem/P3371)
14. [P4667 [BalticOI 2011 Day1] Switch the Lamp On 电路维修](https://www.luogu.com.cn/problem/P4667)
15. [P4779 【模板】单源最短路径（标准版）](https://www.luogu.com.cn/problem/P4779)
16. [P4943 密室](https://www.luogu.com.cn/problem/P4943)
17. [P6822 [PA2012] Tax](https://www.luogu.com.cn/problem/P6822)
18. [矩阵](https://ac.nowcoder.com/acm/problem/253341)
19. [#622. 单源最短路径](https://uoj.ac/problem/622)
20. [Shortest Path](https://judge.yosupo.jp/problem/shortest_path)(https://github.com/yosupo06/library-checker-problems/issues/173)


### 二、模板功能

​	图论模板往往包含一个 `Solver` 和一个 `Graph` 。前者仅仅进行逻辑运算，而不包含图本身的数据；后者保存了图的点、边数据，并提供方便的接口。

​	简单起见，使用者可以只使用 `Graph` 及其接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename Tp` ，表示边权类型。
   
   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。
   
   构造参数 `size_type edge_cnt` ，表示边数。若按有无向边，按两条边计。默认为 `0` 。
   
2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Dijkstra` 算法处理的问题为有向图的最短路问题。

   如果图为无向图，需要将原图的边视为正反两个方向的两条有向边来构建。
   
   本数据结构不能处理带负权的边。

   本数据结构可以接受重边和自环。
   

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type edge_cnt` ，表示边数。若按有无向边，按两条边计。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示边的起点编号。

   输入参数 `size_type b` ，表示边的终点编号。

   输入参数 `Tp dis` ，表示边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**在无向图中，需要按正反两个反向进行加边。

#### 4.获取最短路查询器(calc)

1. 数据类型

   模板参数 `typename Group` ，表示描述路径类型的代数结构。
   
   模板参数 `typename CountType` ，表示最短路计数的类型。
   
   模板参数 `bool GetPath` ，表示在求最短路长度时，是否记录最短路路径。

   输入参数 `size_type source` ，表示起点编号。
   
   输入参数 `size_type target` ，表示终点编号。默认为 `-1` ，表示没有明确终点。

   返回类型 `Solver<Group, CountType, GetPath>` ，表示用来计算和保存最短路的对象。

2. 时间复杂度

   $O(m\cdot\log n)$ 。

3. 备注

   可以通过返回的对象查询最短路长度，生成最短路路径。
   
   如果明确了终点，那么在获取到终点的最短路之后，会立即返回；也就是说比终点更远的点的最短路距离并不保证得到计算。
   
   模板参数 `Group` 规定了边权类型、路径长度类型、路径的默认长度、边权组合成路径长度的方式、路径长度的比较函数类型。若为 `AddGroup` 表示常规的边权和路径长度；若为 `MaxGroup` 表示以最大边权为路径长度。
   
   模板参数 `CountType` 规定了最短路的计数类型。由于最短路往往数量众多，往往传递自取模类型。若传递 `void` ，表示不进行计数。
   
   模板参数 `GetPath` 表示是否保存最短路路径。
   
   **注意：**只有没有零环的情况下可以进行最短路计数；一般来说，仅在正权图、边权和路径长度情况下，才可以进行最短路计数。

#### 5.获取最短路(get_path)

1. 数据类型

   模板参数 `typename Group` ，表示描述路径类型的半群。

   输入参数 `size_type source` ，表示起点编号。

   输入参数 `size_type target` ，表示终点编号。

   返回类型 `std::vector<size_type>` ，表示获取到的最短路。

2. 时间复杂度

   $O(m\cdot\log n)$ 。

3. 备注

   本方法获取从给定起点出发到终点的边权和最小的路径。


### 三、模板示例

```c++
#include "GRAPH/Dijkstra.h"
#include "IO/FastIO.h"
#include "TEST/std_bit.h"

void test_distance_sum() {
    // 普通使用者只需要了解熟悉 OY::Dijkstra::Graph 的使用
    cout << "test distance sum:\n";

    // 建图
    OY::Dijkstra::Graph<int> G(7, 9);
    // 注意加的边都是有向边
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 200);
    G.add_edge(3, 4, 100);
    G.add_edge(3, 5, 100);
    G.add_edge(0, 3, 95);
    G.add_edge(6, 4, 100);
    G.add_edge(4, 5, 190);
    G.add_edge(5, 1, 100);
    G.add_edge(5, 6, 200);

    // 获取最短路长度查询器
    auto table = G.calc(0);
    cout << "min dis from 0 to 0:" << table.query(0) << endl;
    cout << "min dis from 0 to 2:" << table.query(2) << endl;
    cout << "min dis from 0 to 6:" << table.query(6) << endl;

    // 如果模板参数为 true，那么查询器还可以查询最短路的结点编号
    using group = OY::Dijkstra::AddGroup<int>;

    auto table2 = G.calc<group, void, true>(0);
    table2.trace(6, [](int from, int to) { cout << "go from " << from << " -> " << to << endl; });

    // G 本身有更方便的接口
    std::vector<uint32_t> path = G.get_path(0, 6);
    for (int i = 0; i < path.size(); i++) cout << path[i] << (i + 1 == path.size() ? "\n\n" : " -> ");
}

void test_distance_max() {
    cout << "test distance max:\n";

    OY::Dijkstra::Graph<int> G(7, 9);
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 200);
    G.add_edge(3, 4, 100);
    G.add_edge(3, 5, 100);
    G.add_edge(0, 3, 95);
    G.add_edge(6, 4, 100);
    G.add_edge(4, 5, 190);
    G.add_edge(5, 1, 100);
    G.add_edge(5, 6, 200);

    // 定义路径长度为路径中的边长的最大值
    // 获取最短路查询器
    using group = OY::Dijkstra::MaxGroup<int>;
    auto table = G.calc<group>(0);
    cout << "min dis from 0 to 0:" << table.query(0) << endl;
    cout << "min dis from 0 to 2:" << table.query(2) << endl;
    cout << "min dis from 0 to 6:" << table.query(6) << endl;
    cout << endl;
}

void test_count() {
    cout << "test path count:\n";

    OY::Dijkstra::Graph<int> G(4, 5);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(2, 3, 100);
    G.add_edge(0, 2, 300);
    G.add_edge(1, 3, 300);

    // 获取最短路路径数查询器
    using monoid = OY::Dijkstra::AddGroup<int>;
    auto table = G.calc<monoid, int>(0);
    cout << "min dis from 0 to 3:" << table.query(3) << endl;
    cout << "path count:" << table.query_count(3) << endl;
    cout << endl;
}

void test_solver() {
#if CPP_STANDARD >= 201402L
    // 进阶使用者，可以把 Solver 用到自己的图里
    cout << "test solver:\n";
    // 这里以常见的二维 vector 存图举例
    std::vector<std::vector<std::pair<int, int>>> adj(7);
    adj[0].push_back({1, 100});
    adj[0].push_back({2, 200});
    adj[3].push_back({4, 100});
    adj[3].push_back({5, 100});
    adj[0].push_back({3, 95});
    adj[6].push_back({4, 100});
    adj[4].push_back({5, 190});
    adj[5].push_back({1, 100});
    adj[5].push_back({6, 200});

    // 直接建一个可追溯最短路的解答器
    using group = OY::Dijkstra::AddGroup<int, int64_t>;
    OY::Dijkstra::Solver<group, void, true> sol(7);
    sol.set_distance(0, 0);
    // 传递一个遍历边的泛型回调
    sol.run(-1, [&](int from, auto call) {
        for (auto to_and_dis : adj[from]) call(to_and_dis.first, to_and_dis.second);
    });

    // 查询最短路长度
    cout << "min dis from 0 to 0:" << sol.query(0) << endl;
    cout << "min dis from 0 to 2:" << sol.query(2) << endl;
    cout << "min dis from 0 to 6:" << sol.query(6) << endl;

    // 生成一个最短路径
    sol.trace(6, [](int from, int to) { cout << "from " << from << " to " << to << endl; });

#endif
}

int main() {
    test_distance_sum();
    test_distance_max();
    test_count();
    test_solver();
}
```

```
#输出如下
test distance sum:
min dis from 0 to 0:0
min dis from 0 to 2:200
min dis from 0 to 6:395
go from 0 -> 3
go from 3 -> 5
go from 5 -> 6
0 -> 3 -> 5 -> 6

test distance max:
min dis from 0 to 0:0
min dis from 0 to 2:200
min dis from 0 to 6:200

test path count:
min dis from 0 to 3:400
path count:3

test solver:
min dis from 0 to 0:0
min dis from 0 to 2:200
min dis from 0 to 6:395
from 0 to 3
from 3 to 5
from 5 to 6

```
