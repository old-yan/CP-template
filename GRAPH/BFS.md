### 一、模板类别

​	数据结构：`BFS` 算法。

​	练习题目：

1. [LCP 21. 追逐游戏](https://leetcode.cn/problems/Za25hA/)
2. [P1144 最短路计数](https://www.luogu.com.cn/problem/P1144)
3. [P1443 马的遍历](https://www.luogu.com.cn/problem/P1443)
4. [P7293 [USACO21JAN] Sum of Distances P](https://www.luogu.com.cn/problem/P7293)


### 二、模板功能

​	图论模板往往包含一个 `Solver` 和一个 `Graph` 。前者仅仅进行逻辑运算，而不包含图本身的数据；后者保存了图的点、边数据，并提供方便的接口。

​	简单起见，使用者可以只使用 `Graph` 及其接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。
   
   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。
   
   构造参数 `size_type edge_cnt` ，表示边数。若按有无向边，按两条边计。默认为 `0` 。
   
2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `BFS` 算法处理的问题为有向无权图的最短路问题。

   如果图为无向图，需要将原图的边视为正反两个方向的有向边来构建。

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

   模板参数 `bool GetPath` ，表示在求最短路长度时，是否记录最短路路径。

   输入参数 `size_type source` ，表示起点编号。

   输入参数 `const size_type &infinite` ，表示无穷大距离。默认为 `size_type` 类的最大值的一半。

   返回类型 `Solver<GetPath>` ，表示用来计算和保存最短路的对象。

2. 时间复杂度

   $O(m+n)$ 。

3. 备注

   可以通过返回的对象查询最短路长度，以及生成最短路路径。

#### 5.获取最短路(get_path)

1. 数据类型

   输入参数 `size_type source` ，表示起点编号。

   输入参数 `size_type target` ，表示终点编号。

   返回类型 `std::vector<size_type>` ，表示获取到的最短路。

2. 时间复杂度

   $O(m+n)$ 。

3. 备注

   本方法获取从给定起点出发到终点的边数最少的路径。


### 三、模板示例

```c++
#include "GRAPH/BFS.h"
#include "IO/FastIO.h"
#include "TEST/std_bit.h"

void test_bfs() {
    // 普通使用者只需要了解熟悉 OY::BFS::Graph 的使用
    cout << "test bfs:\n";

    // 建图
    OY::BFS::Graph G(7, 9);
    // 注意加的边都是有向边
    G.add_edge(0, 1);
    G.add_edge(0, 2);
    G.add_edge(3, 4);
    G.add_edge(0, 3);
    G.add_edge(6, 4);
    G.add_edge(4, 5);
    G.add_edge(5, 1);
    G.add_edge(3, 5);
    G.add_edge(5, 6);

    // 获取最短路长度查询器
    auto table = G.calc<false>(0);

    cout << "min dis from 0 to 0:" << table.query(0) << endl;
    cout << "min dis from 0 to 2:" << table.query(2) << endl;
    cout << "min dis from 0 to 6:" << table.query(6) << endl;

    // 如果模板参数为 true，那么查询器还可以查询最短路的结点编号
    auto table2 = G.calc<true>(0);
    table2.trace(6, [](int from, int to) {
        cout << "go from " << from << " -> " << to << endl;
    });

    // G 本身有更方便的接口
    std::vector<uint32_t> path = G.get_path(0, 6);
    for (int i = 0; i < path.size(); i++) cout << path[i] << (i + 1 == path.size() ? "\n\n" : " -> ");
}

void test_solver() {
#if CPP_STANDARD >= 201402L
    // 进阶使用者，可以把 Solver 用到自己的图里
    cout << "test solver:\n";
    // 这里以常见的二维 vector 存图举例
    std::vector<std::vector<int>> adj(7);
    adj[0].push_back(1);
    adj[0].push_back(2);
    adj[3].push_back(4);
    adj[0].push_back(3);
    adj[6].push_back(4);
    adj[4].push_back(5);
    adj[5].push_back(1);
    adj[3].push_back(5);
    adj[5].push_back(6);

    // 直接建一个可追溯最短路的解答器
    OY::BFS::Solver<true> sol(7, 9);
    sol.set_distance(0, 0);
    // 传递一个遍历边的泛型回调
    sol.run([&](int from, auto call) {
        for (int to : adj[from]) call(to);
    });

    // 查询最短路长度
    cout << "min dis from 0 to 0:" << sol.query(0) << endl;
    cout << "min dis from 0 to 2:" << sol.query(2) << endl;
    cout << "min dis from 0 to 6:" << sol.query(6) << endl;

    // 生成一个最短路径
    sol.trace(6, [](int from, int to) {
        cout << "from " << from << " to " << to << endl;
    });

#endif
}

int main() {
    test_bfs();
    test_solver();
}
```

```
#输出如下
test bfs:
min dis from 0 to 0:0
min dis from 0 to 2:1
min dis from 0 to 6:3
go from 0 -> 3
go from 3 -> 5
go from 5 -> 6
0 -> 3 -> 5 -> 6

test solver:
min dis from 0 to 0:0
min dis from 0 to 2:1
min dis from 0 to 6:3
from 0 to 3
from 3 to 5
from 5 to 6

```

