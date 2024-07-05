### 一、模板类别

​	数据结构：`Tarjan` 算法。

​	练习题目：

1. [P2863 [USACO06JAN] The Cow Prom S](https://www.luogu.com.cn/problem/P2863)
2. [P3387 【模板】缩点](https://www.luogu.com.cn/problem/P3387)
3. [Strongly Connected Components](https://judge.yosupo.jp/problem/scc)(https://github.com/yosupo06/library-checker-problems/issues/36)

### 二、模板功能

​	图论模板往往包含一个 `Solver` 和一个 `Graph` 。前者仅仅进行逻辑运算，而不包含图本身的数据；后者保存了图的点、边数据，并提供方便的接口。

​	简单起见，使用者可以只使用 `Graph` 及其接口。

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。
   
   构造参数 `size_type vertex_cnt` ，表示点数 。
   
   构造参数 `size_type edge_cnt` ，表示边数。若按有无向边，按两条边计。。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Tarjan` 算法处理的问题为有向图的强连通分量问题。

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

   输入参数 `size_type from`​ ，表示有向边的起点编号。

   输入参数 `size_type to` ，表示有向边的终点编号。

2. 时间复杂度

   $O(1)$ 。

#### 4.获取SCC查询器(calc)

1. 数据类型

   返回类型 `Solver` ，表示用来计算和保存 `SCC` 的对象。

2. 时间复杂度

   $O(m + n)$ 。

3. 备注

   可以通过返回的对象查询 `SCC` 。
   

### 三、模板示例

```c++
#include "GRAPH/Tarjan_scc.h"
#include "IO/FastIO.h"

int main() {
    OY::SCC::Graph G(5, 5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 0);
    G.add_edge(3, 3);
    G.add_edge(3, 1);

    auto sol = G.calc();
    cout << "There are " << sol.group_count() << " groups of SCC\n";
    auto groups = sol.get_groups();
    for (int i = 0; i < groups.size(); i++) {
        cout << "No." << i << " group:\n";
        for (auto a : groups[i]) cout << a << ' ';
        cout << endl;
    }
}
```

```
#输出如下
There are 3 groups of SCC
No.0 group:
4 
No.1 group:
3 
No.2 group:
0 1 2 

```

