### 一、模板类别

​	数据结构： `Gusfield` 算法。

​	练习题目：

1. [P4897 【模板】最小割树（Gomory-Hu Tree）](https://www.luogu.com.cn/problem/P4897)

### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename Tp` ，表示容量、流量的类型。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。无向边计为 `1`  条边。默认为 `0` 。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本模板用于求出无向图的等价流树，可以用于查询无向图上任意两点之间的最小割。

   本模板支持自环和重边，支持不连通的图。

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

   输入参数 `Tp cap` ，表示边的边容量。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本模板的边为无向边，只需从一侧添加一次。

#### 4.计算(calc)

1. 数据类型

   输入参数 `Tp infinite` ，表示无穷大容量，默认为 `Tp` 类型最大值的一半。

2. 时间复杂度

   取决于最大流的算法。若为 `Dinic` 算法，则为 $O(n^3\cdot m)$ 。

#### 5.查询两点间的最小割(query)

1. 数据类型

   输入参数 `size_type a` ，表示无向边的一端的结点编号。

   输入参数 `size_type b` ，表示无向边的一端的结点编号。

   返回类型 `Tp` ，表示两点间的最小割。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   在调用本方法之前，请先调用 `calc` 方法。

### 三、模板示例

```c++
#include "GRAPH/Gusfield.h"
#include "IO/FastIO.h"

int main() {
    // 建图
    OY::Gusfield::Graph<int> G(5, 5);
    // 加五条边
    G.add_edge(0, 1, 200);
    G.add_edge(1, 2, 200);
    G.add_edge(3, 1, 300);
    G.add_edge(3, 2, 100);
    G.add_edge(0, 2, 100);
    // 计算
    G.calc();
    // 查询
    cout << "mincut between 0 and 2 is: " << G.query(0, 2) << endl;
    cout << "mincut between 1 and 4 is: " << G.query(1, 4) << endl;
}
```

```
#输出如下
mincut between 0 and 2 is: 300
mincut between 1 and 4 is: 0

```

