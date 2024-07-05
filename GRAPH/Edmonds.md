### 一、模板类别

​	数据结构：`Edmonds` 算法。

​	练习题目：

1. [P4716 【模板】最小树形图](https://www.luogu.com.cn/problem/P4716)


### 二、模板功能

​	图论模板往往包含一个 `Solver` 和一个 `Graph` 。前者仅仅进行逻辑运算，而不包含图本身的数据；后者保存了图的点、边数据，并提供方便的接口。

​	简单起见，使用者可以只使用 `Graph` 及其接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename Tp` ，表示边权类型。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Edmonds` 算法处理的问题为最小树形图问题，即有向图的最小生成树问题。

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

   输入参数 `size_type a`​ ，表示边的起点编号。

   输入参数 `size_type b` ，表示边的终点编号。

   输入参数 `Tp cost` ，表示边权。

2. 时间复杂度

   $O(1)$ 。


#### 4.获取生成树查询器(calc)

1. 数据类型

   模板参数 `bool GetPath` ，表示在求最小树形图时，是否记录树边。
   
   模板参数 `typename SumType` ，表示边权和的类型。默认为 `Tp` 。

   输入参数 `size_type root` ，表示起点编号。

   输入参数 `const SumType &infinite` ，表示无穷大代价。默认为 `SumType` 类的最大值的一半。

   返回类型 `std::pair<Solver<Tp, SumType, GetPath>, bool>` ，前者表示用来计算和保存生成树的对象，后者表示是否生成成功。

2. 时间复杂度

   $O(m\cdot n)$ 。

3. 备注

   如果生成成功，可以通过返回的对象查询最小生成代价，生成树边。

### 三、模板示例

```c++
#include "GRAPH/Edmonds.h"
#include "IO/FastIO.h"

int main() {
    OY::Edmonds::Graph<uint32_t> G(5, 6);
    G.add_edge(2, 3, 100);
    G.add_edge(2, 4, 120);
    G.add_edge(0, 2, 10);
    G.add_edge(0, 1, 180);
    G.add_edge(0, 4, 200);
    G.add_edge(4, 1, 50);

    auto res = G.calc<true>(0);
    auto &&sol = res.first;
    bool flag = res.second;
    if (!flag) {
        cout << "There is no Tree\n";
    } else {
        cout << "There is Tree\n";
        cout << "Tree total cost: " << sol.total_cost() << endl;
        cout << "used edges:\n";
        sol.do_for_used_edges([&](int index) {
            uint32_t from = G.m_edges[index].m_from, to = G.m_edges[index].m_to;
            auto cost=G.m_edges[index].m_cost;
            cout << "use No." << index << " edge, from " << from << " to " << to << ", cost = " << cost << endl;
        });
    }
}
```

```
#输出如下
There is Tree
Tree total cost: 280
used edges:
use No.0 edge, from 2 to 3, cost = 100
use No.1 edge, from 2 to 4, cost = 120
use No.2 edge, from 0 to 2, cost = 10
use No.5 edge, from 4 to 1, cost = 50

```

