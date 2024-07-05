### 一、模板类别

​	数据结构：二分图检验。

​	练习题目：

1. [P1330 封锁阳光大学](https://www.luogu.com.cn/problem/P1330)
2. [U248878 染色法判定二分图](https://www.luogu.com.cn/problem/U248878)


### 二、模板功能

​	图论模板往往包含一个 `Solver` 和一个 `Graph` 。前者仅仅进行逻辑运算，而不包含图本身的数据；后者保存了图的点、边数据，并提供方便的接口。

​	简单起见，使用者可以只使用 `Graph` 及其接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `size_type edge_cnt` ，表示边数。默认为 `0` 。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本数据结构用于检验一个有向图是否为二分图。

   本数据结构可以接受重边和自环。当然，一旦出现自环，一定不是二分图。
   

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   输入参数 `size_type edge_cnt` ，表示边数。默认为 `0` 。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示边的起点编号。

   输入参数 `size_type b` ，表示边的终点编号。

2. 时间复杂度

   $O(1)$ 。


#### 4..获取二分图查询器(calc)

1. 数据类型

   返回类型 `std::pair<Solver, bool>` ，前者表示用来计算和保存染色结果的对象，后者表示是否为二分图。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本算法会使用染色法，尝试将整个图染色，使得每条边的两端颜色不同。

   本算法会对每个连通块进行独立染色，如图中有三块连通区域，则第一块的结点会被染为 `0` 和 `1` ，第二块的结点会被染为 `2` 和 `3` ，第三块的结点会被染为 `4` 和 `5` 。任何一个连通块染色失败，都将使整个图不被认定为二分图。

### 三、模板示例

```c++
#include "GRAPH/Bipartite.h"
#include "IO/FastIO.h"

int main() {
    OY::Bipartite::Graph G(7, 4);

    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(6, 5);

    auto res = G.calc();
    auto &&sol = res.first;
    bool flag = res.second;
    // 查看染色结果
    for (int i = 0; i < 7; i++) cout << sol.query(i) << " \n"[i == 5];

    if (!flag)
        cout << "It is not bipartite\n";
    else
        cout << "It is bipartite\n";

    if (sol.in_same_group(0, 3)) cout << "0 and 3 is in same group\n";

    if (!sol.in_same_group(5, 3)) cout << "5 and 3 is not in same group\n";
}
```

```
#输出如下
0 1 0 1 2 4
5 It is bipartite
0 and 3 is in same group
5 and 3 is not in same group

```

