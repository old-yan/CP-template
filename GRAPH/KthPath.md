### 一、模板类别

​	数据结构：`K` 短路。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权的类型。

   模板参数 `bool _PassTarget` ，表示在到达终点之后是否还能出去溜达一圈再回来。

   构造参数 `uint32_t __vertexNum`​ ，表示图中的点数。

   构造参数 `uint32_t __edgeNum` ，表示图中预备要存的边数。

   构造参数 `_Tp __infiniteDistance` ，表示无穷大边权值。默认为 `_Tp` 类的最大值的一半。
   
2. 时间复杂度

   $O(n+m)$ 。

3. 备注

    `K` 短路算法处理的问题为第 `k` 短的路有多长的问题。

   本数据结构要求边权为正。
   
   本数据结构可以接受重边和自环。

   **注意：**
   
   本数据结构一开始指定的 `__vertexNum` 参数必须是确切值。
   
   一开始指定的 `__edgeNum` 参数可以是模糊值，是用来为加边预留空间的，即使一开始没有留够空间，也可以自动扩容。如果边数不确定，可以预估一个上限填进去。对于无向图，本参数须为图的边数的二倍。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示有向边的起点编号。

   输入参数 `uint32_t __b` ，表示有向边的终点编号。

   输入参数 `_Tp __cost` ，表示有向边的边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**在无向图中，需要按正反两个反向进行加边。

#### 3.设置起点终点

1. 数据类型

   输入参数 `uint32_t __source` ，表示起点。

   输入参数 `uint32_t __target` ，表示终点。

2. 时间复杂度

   $O(1)$ 。

#### 4.预备

1. 数据类型

2. 时间复杂度

   $O((n+m)\cdot\log n)$ 。

3. 备注

   本方法用于在加完所有边后，进行一些预处理。

#### 5.取出下一条最短的路长度

1. 数据类型

   返回类型 `_Tp` ，表示下一条最短的路的长度。

2. 时间复杂度

   $O(\log k)$ ，此处 `k` 表示当前调用次数。
   
3. 备注

   本方法可以多次调用，显然，随着本函数的调用次数增加，返回值单调不减。
   
   如果已经枚举完所有的可行路线，接下来的返回值均为 `m_infiniteDistance` 值。

### 三、模板示例

```c++
#include "GRAPH/KthPath.h"
#include "IO/FastIO.h"

int main() {
    // k短路初始化
    OY::KthPath<long long> kp(7, 9);

    // 加边
    kp.addEdge(0, 1, 1);
    kp.addEdge(1, 2, 1);
    kp.addEdge(2, 3, 1);
    kp.addEdge(3, 4, 1);
    kp.addEdge(4, 5, 1);
    kp.addEdge(5, 6, 1);
    kp.addEdge(3, 4, 210);
    kp.addEdge(4, 5, 200);
    kp.addEdge(4, 5, 220);

    kp.setting(0, 6);
    kp.prepare();

    for (int i = 0;; i++) {
        long long length = kp.next();
        if (length == kp.m_infiniteDistance) break;
        cout << "No." << i << " path length: " << length << endl;
    }
}
```

```
#输出如下
No.0 path length: 6
No.1 path length: 205
No.2 path length: 215
No.3 path length: 225
No.4 path length: 414
No.5 path length: 434

```

