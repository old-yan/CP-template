### 一、模板类别

​	数据结构：`Steiner` 算法。（最小边权和）

​	练习题目：

1. [P4294 [WC2008] 游览计划](https://www.luogu.com.cn/problem/P4294)
2. [P6192 【模板】最小斯坦纳树](https://www.luogu.com.cn/problem/P6192)


### 二、模板功能

​	图论模板往往包含一个 `Solver` 和一个 `Graph` 。前者仅仅进行逻辑运算，而不包含图本身的数据；后者保存了图的点、边数据，并提供方便的接口。

​	简单起见，使用者可以只使用 `Graph` 及其接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename EdgeCostType` ，表示边权类型。
   
   模板参数 `typename VertexCostType` ，表示点权类型。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。无向边按一条边计。默认为 `0` 。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Steiner` 算法处理的问题，与无向图的最小生成树问题很接近。区别就是，`Steiner` 算法允许在要连通的点集之外，选取一些额外的点，也就是这些第三方的点并不要求被连通，但是可以提供连通协助。
   
   本模板既可以处理边权和最小问题，也可以处理点权和最小问题，也可以边权和点权同时处理。当没有边权时，将 `EdgeCostType` 设为 `void` ；当没有点权时，将 `VertexCostType` 设为 `void` ；但二者不能同时为 `void` 。

   本数据结构可以接受重边和自环。
   
   **注意：**
   
   本模板要求在运行 `calc` 之前，至少指定一个关键点。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type edge_cnt` ，表示边数。无向边按一条边计。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示边的起点编号。

   输入参数 `size_type b` ，表示边的终点编号。

   输入参数 `EdgeCostType cost` ，表示边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在无向图中，只需要按一侧方向加边。
   
   当边权类型为 `void` 时，不需要传递参数三。

#### 4.指定关键点(set_key)

1. 数据类型

   输入参数 `size_type i` ，表示要指定的关键结点的编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   关键结点，即之后要选边保证连通的结点。

#### 5.指定点值(set_value)

1. 数据类型

   输入参数 `size_type i` ，表示要指定的结点的编号。
   
   输入参数 `VertexCostType val` ，表示结点权值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   注意，指定权值的结点可以是普通结点，不一定是关键点。
   
   本方法仅当点权类型不为 `void` 时使用。

#### 6.获取生成树查询器(calc_dijk)

1. 数据类型

   模板参数 `bool GetPath` ，表示在求最小斯坦纳树时，是否记录树边。
   
   模板参数 `typename SumType` ，表示权值和类型。

   输入参数 `const SumType &infinite` ，表示无穷大代价。默认为 `SumType` 类的最大值的一半。

   返回类型 `std::pair<Solver<Tp, SumType, GetPath>, bool>` ，前者表示用来计算和保存生成树的对象，后者表示生成是否成功。

2. 时间复杂度

    $O(n\cdot 3^k+m\cdot \log n\cdot 2^k)$ 。

3. 备注

   如果未连通，则计算失败。

   如果连通，则计算成功，可以通过返回的对象查询最小生成代价，生成树边。

#### 7.获取生成树查询器(calc_spfa)

1. 数据类型

   模板参数 `bool GetPath` ，表示在求最小斯坦纳树时，是否记录树边。

   模板参数 `typename SumType` ，表示权值和类型。

   输入参数 `const SumType &infinite` ，表示无穷大代价。默认为 `SumType` 类的最大值的一半。

   返回类型 `std::pair<Solver<Tp, SumType, GetPath>, bool>` ，前者表示用来计算和保存生成树的对象，后者表示生成是否成功。

2. 时间复杂度

    $O(n\cdot 3^k+m\cdot n\cdot 2^k)$ 。

3. 备注

   如果未连通，则计算失败。

   如果连通，则计算成功，可以通过返回的对象查询最小生成代价，生成树边。

   使用 `SPFA` 算法的时间复杂度比 `Dijkstra` 版的要高，但实际跑起来有可能比 `Dijkstra` 版更快。当然一般情况下 `Dijkstra` 的速度已经够了。

### 三、模板示例

```c++
#include "GRAPH/Steiner.h"
#include "IO/FastIO.h"

void test_int_void() {
    // 无向图
    OY::STEINER::Graph<int> G(6, 8);
    // 加八条边
    G.add_edge(0, 1, 100);
    G.add_edge(1, 2, 120);
    G.add_edge(2, 0, 110);
    G.add_edge(0, 3, 20);
    G.add_edge(4, 3, 10);
    G.add_edge(4, 2, 50);
    G.add_edge(4, 5, 10);
    G.add_edge(5, 1, 30);
    // 指定三个关键点
    G.set_key(0);
    G.set_key(1);
    G.set_key(2);

    // 计算可能性
    auto res = G.calc_spfa<true>();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag)
        cout << "There is no Steiner Tree\n";
    else {
        cout << "There is Steiner Tree\n";
        cout << "total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";
        sol.do_for_used_edges([&](int index) {
            int from = G.m_edges[index].m_from;
            int to = G.m_edges[index].m_to;
            int cost = G.m_edges[index].m_cost;
            cout << "use No." << index << " edge, from " << from << " to " << to << ", cost = " << cost << endl;
        });
    }
}

void test_void_int() {
    // 无向图
    OY::STEINER::Graph<void, int> G(6, 8);
    // 加八条边
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(0, 3);
    G.add_edge(4, 3);
    G.add_edge(4, 2);
    G.add_edge(4, 5);
    G.add_edge(5, 1);
    // 指定三个关键点
    G.set_key(2);
    G.set_key(3);
    G.set_key(5);
    // 指定每个点的权值
    // 不管怎么选，都会把 2 3 5 选中，所以 2 3 5 的权值是多少并不重要
    // 有两种联通方案：[2,3,5,4] [2,3,5,0,1]
    // 只要把 4 的权值设大一点，就会强迫选后者
    G.set_value(4, 100);
    G.set_value(0, 20);
    G.set_value(1, 50);

    // 计算可能性
    auto res = G.calc_spfa<true>();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag)
        cout << "There is no Steiner Tree\n";
    else {
        cout << "There is Steiner Tree\n";
        cout << "total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";
        sol.do_for_used_edges([&](int index) {
            int from = G.m_edges[index].m_from;
            int to = G.m_edges[index].m_to;
            cout << "use No." << index << " edge, from " << from << " to " << to << endl;
        });
    }
}

int main() {
    test_int_void();
    test_void_int();
}
```

```
#输出如下
There is Steiner Tree
total cost: 120
used edges:
use No.3 edge, from 0 to 3, cost = 20
use No.4 edge, from 4 to 3, cost = 10
use No.5 edge, from 4 to 2, cost = 50
use No.6 edge, from 4 to 5, cost = 10
use No.7 edge, from 5 to 1, cost = 30
There is Steiner Tree
total cost: 70
used edges:
use No.2 edge, from 2 to 0
use No.0 edge, from 0 to 1
use No.3 edge, from 0 to 3
use No.7 edge, from 5 to 1

```

