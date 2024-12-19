### 一、模板类别

​	数据结构：动态 `Floyd` 算法。

​	练习题目：

1. [Shortest Path](https://acm.hdu.edu.cn/showproblem.php?pid=3631)
2. [2642. 设计可以求最短路径的图类](https://leetcode.cn/problems/design-graph-with-shortest-path-calculator/)
3. [P2683 小岛](https://www.luogu.com.cn/problem/P2683)
4. [P7516 [省选联考 2021 A/B 卷] 图函数](https://www.luogu.com.cn/problem/P7516)
5. [C. 最短路径](https://qoj.ac/contest/1875/problem/9903)


### 二、模板功能


#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename Group` ，表示一个代数结构。

   模板参数 `bool GetPath` ，表示是否获取最短路的路径。
   
   模板参数 `template <typename> typename Container` ，表示使用什么容器进行数据存储。默认为 `vector` 存储方式。

   构造参数 `size_type vertex_cnt` ，表示点数。

2. 时间复杂度

   $O(n*n)$ 。

3. 备注

   `Floyd` 算法处理的问题为有向图的最短路问题。

   本数据结构可以接受重边和自环。
   
   模板参数 `Group` 规定了边权类型、路径长度类型、路径的默认长度、边权组合成路径长度的方式、路径长度的比较函数类型。若为 `AddGroup` 表示常规的边权和路径长度；若为 `MaxGroup` 表示以最大边权为路径长度。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

2. 时间复杂度

   $O(n*n)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type from`​ ，表示边的起点编号。

   输入参数 `size_type to` ，表示边的终点编号。

   输入参数 `value_type val` ，表示边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   调用本函数之后，并未对各条最短路进行更新；所以此时 `Solver` 无法查询最短路，如果进行查询，查询出的最短路可能是错误的。只有调用 `run` 或者 `update_by_vertex` 之后，将加的边的影响更新，才可以进行正确的查询，

#### 4.全局跑最短路(run)

1. 数据类型

   输入参数 `Callback &&call` ，表示当某个点对的最短距离被更新时，调用的回调函数。默认为 `Ignore` 类的默认实例。

2. 时间复杂度

    $O(n^3)$ 。

3. 备注

   本方法将所有的边统计进来，计算所有点对之间的最短路。
   
   本方法实质为，对所有点调用 `update_by_vertex` 。

#### 5.加边并通过这条边更新(update_by_edge)

1. 数据类型

   输入参数 `size_type from`​ ，表示边的起点编号。

   输入参数 `size_type to` ，表示边的终点编号。

   输入参数 `value_type val` ，表示边权。
   
   输入参数 `Callback &&call` ，表示当某个点对的最短距离被更新时，调用的回调函数。默认为 `Ignore` 类的默认实例。

2. 时间复杂度

    $O(n^2)$ 。

3. 备注

   本方法与 `add_edge` 的区别是，本方法加边之后，会将这条边的影响更新到图里，所以之后立刻进行最短路查询也是合格的。

#### 6.通过点更新(update_by_vertex)

1. 数据类型

   输入参数 `size_type k`​ ，表示要由之更新的结点的编号。
   
   输入参数 `Callback &&call` ，表示当某个点对的最短距离被更新时，调用的回调函数。默认为 `Ignore` 类的默认实例。

2. 时间复杂度

    $O(n^2)$ 。

3. 备注

   本方法并不添加任何新边；所以在调用本方法之前，往往是通过 `add_edge` 添加若干条与 `k` 相关的边，再调用本方法，由结点 `k` 进行更新。

#### 7.查询最短路(query)

1. 数据类型

   输入参数 `size_type from`​ ，表示起点的编号。
   
   输入参数 `size_type to`​ ，表示终点的编号。
   
   返回类型 `value_type` ，表示最短路长度。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   当最短路不存在时，返回结果恰等于 `infinite()` 。

#### 8.查询无穷距离(infinite)

1. 数据类型
   
   返回类型 `value_type` ，表示查询结果。

2. 时间复杂度

    $O(1)$ 。

### 三、模板示例

```c++
#include "GRAPH/DynamicFloyd.h"
#include "IO/FastIO.h"

void test_run() {
    OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<int>> G(4);
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
    OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<int>> G(4);
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

void test_update_by_vertex() {
    OY::VectorDynamicFloyd<OY::DFLOYD::AddGroup<int>> G(4);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 0, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(1, 3, 300);
    G.add_edge(1, 2, 10);
    G.add_edge(2, 3, 50);
    
    // 也可以手动调用“对点更新”
    // 更新顺序可以随便
    G.update_by_vertex(2);
    G.update_by_vertex(0);
    G.update_by_vertex(3);
    G.update_by_vertex(1);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) cout << (G.query(i, j) == G.infinite() ? -1 : G.query(i, j)) << " \n"[j == 3];
    cout << endl;
}

int main() {
    test_run();
    test_update_by_edge();
    test_update_by_vertex();
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

0 100 110 160
100 0 10 60
-1 -1 0 50
-1 -1 -1 0

```

