### 一、模板类别

​	数据结构：`StoerWagner` 算法。

​	练习题目：

1. [P5632 【模板】Stoer-Wagner](https://www.luogu.com.cn/problem/P5632)

### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `type_name Tp` ，表示边权类型。

   构造参数 `size_type vertex_cnt` ，表示点数。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   `StoerWagner` 算法处理的问题为无向图的全局最小割，即将全图分为两部分，使得横跨两部分的边权和最小。

   本数据结构可以接受重边和自环。
   

#### 2.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示无向边的起点编号。

   输入参数 `size_type b` ，表示无向边的终点编号。

   输入参数 `Tp cost` ，表示无向边的边权。

2. 时间复杂度

   $O(1)$ 。


#### 3.计算全局最小割(calc)

1. 数据类型

   模板参数 `bool GetPath` ，表示是否记录最小割方案。
   
   模板参数 `typename SumType` ，表示代价和的类型。默认为 `Tp` 。

   输入参数 `const SumType &infinite` ，表示无穷大边权值。默认为 `SumType` 类的最大值的一半。

   返回类型 `SumType`，表示全局最小割的值。

2. 时间复杂度

   $O(n^3)$ 。

#### 4.查询某点是否被选中(is_chosen)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的结点编号。

   返回类型 `bool` ，表示该结点是否被选中。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   最小割本质上就是对整个图的结点进行染色，染成两种颜色。如果一条边两端的结点颜色不同，则这条边即为最小割的一部分。

   本方法返回的真假，可以将所有的结点划分为两种颜色。

### 三、模板示例

```c++
#include "GRAPH/StoerWagner.h"
#include "IO/FastIO.h"

int main() {
    // 无向图
    OY::StoerWagner::Graph<int> G(5);
    // 加六条边
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 20);
    G.add_edge(1, 3, 20);
    G.add_edge(2, 3, 40);
    G.add_edge(4, 3, 20);
    G.add_edge(3, 0, 50);
    // 计算最小割
    cout << "min cut is :" << G.calc<true>() << endl;
    // 输出最小割方案
    cout << "first part:\n";
    for (int i = 0; i < 5; i++)
        if (G.is_chosen(i)) cout << i << " ";
    cout << "\nsecond part:\n";
    for (int i = 0; i < 5; i++)
        if (!G.is_chosen(i)) cout << i << " ";
}
```

```
#输出如下
min cut is :20
first part:
4 
second part:
0 1 2 3 
```

