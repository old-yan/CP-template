### 一、模板类别

​	数据结构： `KM` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Tp` ，表示图中边权类型。

   模板参数 `uint32_t _MAXN` ，表示图中可能的最多点数（左侧点数和右侧点数取最大）。

   构造参数 `uint32_t __leftNum`​ ，表示图中左侧集合的点数。

   构造参数 `uint32_t __rightNum` ，表示图中右侧集合的点数。

   构造参数 `_Tp __initValue` ，表示初始边权，默认为 `0` 。

   构造参数 `_Tp __infiniteValue` ，表示无穷大边权，默认为 `_Tp` 类的最大值的一半。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

    `KM` 算法处理的问题为二分图最大权完美匹配。

   本数据结构可以接受重边。

   本数据结构没有自环概念。

   **注意：**

   本数据结构一开始指定的 `__leftNum` 和 `__rightNum` 参数必须是确切值。

   由于存图采取的是邻接矩阵方法，所以无须对边数进行预估。

   设置初始边权用于解决不同类型问题。

#### 2.加边

1. 数据类型

   输入参数 `uint32_t __a`​ ，表示边的起点在左侧集合中的编号。

   输入参数 `uint32_t __b` ，表示边的终点在右侧集合中的编号。

   输入参数 `_Tp __value` ，表示边权。

2. 时间复杂度

   $O(1)$。

#### 3.求最大权完美匹配

1. 数据类型

   返回类型 `_Tp` ，表示求出的最大匹配值。

2. 时间复杂度

   $O(n^3)$ 。

### 三、模板示例

```c++
#include "GRAPH/KuhnMunkres.h"
#include "IO/FastIO.h"

int main() {
    //建立二分图
    OY::KuhnMunkres<int, 10> G(3, 3, 0);
    //加五条边，G.addEdge(a,b,value) 表示男孩 a 喜欢女孩 b
    G.addEdge(0, 1, 10);
    G.addEdge(1, 2, 10);
    G.addEdge(2, 1, 8);
    G.addEdge(2, 2, 9);
    G.addEdge(1, 1, 12);
    //求最大权完美匹配
    cout << "max match = " << G.calc() << endl;
    //看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "boy " << i << " matches girl " << int(G.m_leftMatch[i]) << endl;
    }
    //看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "girl " << i << " matches boy " << int(G.m_rightMatch[i]) << endl;
    }
}
```

```
#输出如下
max match = 21
boy 0 matches girl 0
boy 1 matches girl 1
boy 2 matches girl 2
girl 0 matches boy 0
girl 1 matches boy 1
girl 2 matches boy 2

```

