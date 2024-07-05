### 一、模板类别

​	数据结构： `Hopcroft Karp` 算法。

​	练习题目：

1. [P3386 【模板】二分图最大匹配](https://www.luogu.com.cn/problem/P3386)
2. [#78. 二分图最大匹配](https://uoj.ac/problem/78)
3. [Matching on Bipartite Graph](https://judge.yosupo.jp/problem/bipartitematching)(https://github.com/yosupo06/library-checker-problems/issues/37)


### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。默认为 `0` 。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

    `Hopcroft Karp` 算法处理的问题为二分图的最大匹配。

   本数据结构可以接受重边。
   
   本数据结构没有自环概念。

   **注意：**
   
   本模板认为二分图的左侧结点数和右侧结点数为同阶的量。如果遇到左侧结点数不等于右侧结点数的情况，结点数填写二者的较大值。

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

   输入参数 `size_type left`​ ，表示一条边的左侧结点编号。

   输入参数 `size_type right` ，表示有向边的右侧结点编号。

2. 时间复杂度

   $O(1)$ 。

#### 4.计算最大匹配(calc)

1. 数据类型

   返回类型 `size_type` ，表示求得的最大匹配值。

2. 时间复杂度

   $O(m\cdot\sqrt n)$ 。

#### 5.查询左侧结点匹配的右侧结点(find_right)

1. 数据类型

   输入参数 `size_type left` ，表示左侧结点编号。

   返回类型 `size_type` ，表示其匹配的右侧结点。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在进行查询前，须先调用 `calc` 方法。

#### 6.查询右侧结点匹配的左侧结点(find_left)

1. 数据类型

   输入参数 `size_type right` ，表示右侧结点编号。

   返回类型 `size_type` ，表示其匹配的左侧结点。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在进行查询前，须先调用 `calc` 方法。


### 三、模板示例

```c++
#include "GRAPH/HopcroftKarp.h"
#include "IO/FastIO.h"

int main() {
    // 建立二分图
    OY::HK::Graph G(3, 5);
    // 加五条边，G.addEdge(a,b) 表示男孩 a 喜欢女孩 b
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 1);
    G.add_edge(2, 2);
    G.add_edge(1, 1);
    // 求最大匹配
    cout << "max match = " << G.calc() << endl;
    // 看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "boy " << i << " matches girl " << int(G.find_right(i)) << endl;
    }
    // 看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "girl " << i << " matches boy " << int(G.find_left(i)) << endl;
    }
}
```

```
#输出如下
max match = 2
boy 0 matches girl 1
boy 1 matches girl 2
boy 2 matches girl -1
girl 0 matches boy -1
girl 1 matches boy 0
girl 2 matches boy 1

```

