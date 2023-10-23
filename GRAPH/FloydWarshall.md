### 一、模板类别

​	数据结构：`Floyd-Warshall` 算法。

​	练习题目：

1. [B3611 【模板】传递闭包](https://www.luogu.com.cn/problem/B3611)


### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中的编号类型。

   模板参数 `size_type MAX_VERTEX` ，表示图中可能的最大结点数。

   构造参数 `size_type vertex_cnt`​ ，表示本图的实际点数。

2. 时间复杂度

   $O(\frac{n^2}{w})$ ，此处 `w` 表示 `std::bitset` 运算时的字长。

3. 备注

   `Floyd-Warshall` 算法处理的问题为有向图的闭包传递问题。通俗地讲，就是可达性问题。

   如果图为无向图，需要将原图的边视为正反两个方向的有向边来构建。
   
   本数据结构可以接受重边和自环。

   **注意：**
   
   由于存图采取的是邻接矩阵方法，所以无须对边数进行预估。
   
   **注意：**
   
   本模板的模板参数与其余模板的不同， `MAX_VERTEX` 表示的是单次样例中的最大结点数。例如，某个测试点有十组数据，每组的结点数均为 `100` 。那么 `MAX_VERTEX` 应当设为 `100` 而非 `1000` 。

#### 2.加边

1. 数据类型

   输入参数 `size_type a`​ ，表示有向边的起点编号。

   输入参数 `size_type b` ，表示有向边的终点编号。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**在无向图中，需要按正反两个反向进行加边。

#### 3.计算可达性

1. 数据类型

2. 时间复杂度

   $O(\frac{n^3}{w})$ 。


### 三、模板示例

```c++
#include "GRAPH/FloydWarshall.h"
#include "IO/FastIO.h"

int main() {
    // 有向图
    OY::FloydWarshall::Graph<100> G(3);
    // 加边
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(0, 2);
    G.add_edge(2, 1);
    // 求可达性
    G.calc();
    if (G.m_can[0][2]) {
        cout << "0 can reach 2\n";
    } else {
        cout << "0 can't reach 2\n";
    }
    if (G.m_can[2][0]) {
        cout << "2 can reach 0\n";
    } else {
        cout << "2 can't reach 0\n";
    }
}
```

```
#输出如下
0 can reach 2
2 can't reach 0

```

