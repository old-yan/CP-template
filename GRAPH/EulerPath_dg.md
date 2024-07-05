### 一、模板类别

​	数据结构：有向图的欧拉回路。

​	练习题目：

1. [P1127 词链](https://www.luogu.com.cn/problem/P1127)
2. [P7771 【模板】欧拉路径](https://www.luogu.com.cn/problem/P7771)
3. [#117. 欧拉回路](https://uoj.ac/problem/117)
4. [Eulerian Trail (Directed)](https://judge.yosupo.jp/problem/eulerian_trail_directed)(https://github.com/yosupo06/library-checker-problems/issues/25)


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

   本算法处理的问题为有向图的欧拉回路和欧拉通路问题。

   本数据结构可以接受重边和自环。
   
   **注意：**
   
   本数据结构要求至少有一条边。
   
#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type edge_cnt` ，表示边数。若按有无向边，按两条边计。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示有向边的起点编号。

   输入参数 `size_type b` ，表示有向边的终点编号。

2. 时间复杂度

   $O(1)$ 。

#### 4.获取欧拉路查询器(calc)

1. 数据类型

   返回类型 `Solver` ，表示用来计算和保存欧拉路的对象。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   可以通过返回的对象查询图是否为欧拉图/半欧拉图，生成欧拉路。

#### 5.获取欧拉路(get_path)

1. 数据类型

   输入参数 `size_type prefer_source` ，表示如果为欧拉图时，优先选择的起点。默认为 `0` 。

   返回类型 `std::vector<size_type>` ，表示获取到的欧拉路。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   本方法获取欧拉路径。

   当图为欧拉图时，任何一点都可以为起点，所以一定会以 `prefer_source` 为起点。

   当图为半欧拉图时，起点为确定点，所以不一定会以 `prefer_source` 为起点。


### 三、模板示例

```c++
#include "GRAPH/EulerPath_dg.h"
#include "IO/FastIO.h"

int main() {
    OY::EulerPathDG::Graph G(5, 5);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 4);
    G.add_edge(4, 1);
    G.add_edge(1, 3);

    auto sol = G.calc(0);

    if (sol.is_Euler_graph()) {
        cout << "Graph is EulerGraph\n";
        sol.trace([](int index, int from, int to) {
            cout << "No." << index << " edge, from " << from << " to " << to << endl;
        });
    } else if (sol.is_half_Euler_graph()) {
        cout << "Graph is half EulerGraph\n";
        sol.trace([](int index, int from, int to) {
            cout << "No." << index << " edge, from " << from << " to " << to << endl;
        });
    } else {
        cout << "Graph isn't EulerGraph\n";
    }
}
```

```
#输出如下
Graph is half EulerGraph
No.0 edge, from 0 to 1
No.1 edge, from 1 to 2
No.2 edge, from 2 to 4
No.3 edge, from 4 to 1
No.4 edge, from 1 to 3

```

