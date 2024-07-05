### 一、模板类别

​	数据结构：拓扑排序算法。

​	练习题目：

1. [B3644 【模板】拓扑排序 / 家谱树](https://www.luogu.com.cn/problem/B3644)


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

   本数据结构可以接受重边和自环。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type edge_cnt` ，表示边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示有向边的起点编号。

   输入参数 `size_type b` ，表示有向边的终点编号。

2. 时间复杂度

   $O(1)$ 。

#### 4.获取拓扑查询器(calc)

1. 数据类型

   返回类型 `std::pair<Solver, bool>` ，前者表示用来计算和保存拓扑序的对象，后者表示是否成功进行拓扑排序。

2. 时间复杂度

   $O(m+n)$ 。

3. 备注

   可以通过返回的对象查询拓扑路径。

#### 5.获取最短路(get_path)

1. 数据类型

   返回类型 `std::vector<size_type>` ，表示获取到的最短路。

2. 时间复杂度

   $O(m+n)$ 。

3. 备注

   本方法获取拓扑路径。


### 三、模板示例

```c++
#include "GRAPH/TopologicalSort.h"
#include "IO/FastIO.h"

int main() {
    OY::TOPO::Graph G(5, 5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 4);
    G.add_edge(1, 4);
    G.add_edge(3, 4);

    auto res = G.calc();
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag) {
        cout << "There is cycle in the graph\n";
    } else {
        cout << "There is no cycle in the graph\n";
        sol.trace([](int i) {
            cout << i << ' ';
        });
    }
}
```

```
#输出如下
There is no cycle in the graph
0 3 1 2 4 

```

