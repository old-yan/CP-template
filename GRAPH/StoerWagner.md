### 一、模板类别

​	数据结构：`StoerWagner` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   模板参数 `uint32_t _MAXN` ，表示图中可能的最多点数。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的实际点数。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   `StoerWagner` 算法处理的问题为无向图的全局最小割，即将全图分为两部分，使得横跨两部分的边权和最小。

   本数据结构可以接受重边和自环。
   
   **注意：**

   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   由于存图采取的是邻接矩阵方法，所以无须对边数进行预估。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示无向边的起点编号。

   输入参数 `uint32_t __b` ，表示无向边的终点编号。

   输入参数 `_Tp __cost` ，表示无向边的边权。

2. 时间复杂度

   $O(1)$。


#### 3.计算全局最小割

1. 数据类型

   模板参数 `bool _GetPath` ，表示是否记录最小割方案。

   输入参数 `_Tp __infiniteCost` ，表示无穷大边权值。默认为 `_Tp` 类的最大值的一半。

   返回类型 `_Tp`，表示全局最小割的值。

2. 时间复杂度

   $O(n^3)$ 。

3. 备注

   如果记录了最小割方案，可以使用 `m_rest` 属性查看全局最小割时剩余部分的点。

### 三、模板示例

```c++
#include "GRAPH/StoerWagner.h"
#include "IO/FastIO.h"

int main() {
    //无向图
    OY::StoerWagner<int, 100> G(5);
    //加六条边
    G.addEdge(0, 1, 100);
    G.addEdge(0, 2, 20);
    G.addEdge(1, 3, 20);
    G.addEdge(2, 3, 40);
    G.addEdge(4, 3, 20);
    G.addEdge(3, 0, 50);
    //计算最小割
    cout << "min cut is :" << G.calc<true>() << endl;
    //输出最小割方案
    cout << "first part:\n";
    for (uint32_t a = G.m_rest._Find_first(); a < 5; a = G.m_rest._Find_next(a)) cout << a << ' ';
    cout << "\nsecond part:\n";
    auto other = G.m_rest;
    other.flip();
    for (uint32_t a = other._Find_first(); a < 5; a = other._Find_next(a)) cout << a << ' ';
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

