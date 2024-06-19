### 一、模板类别

​	数据结构：`Bron-Kerbosch` 算法。

​	练习题目：

1. [Maximum Clique](https://acm.hdu.edu.cn/showproblem.php?pid=1530)
2. [P4212 外太空旅行](https://www.luogu.com.cn/problem/P4212)
3. [Maximum Independent Set](https://judge.yosupo.jp/problem/maximum_independent_set)(https://github.com/yosupo06/library-checker-problems/issues/151)


### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中的编号类型。

   模板参数 `typename MaskType` ，表示掩膜类型。

   模板参数 `size_type MAX_VERTEX` ，表示图中可能的最大结点数。

   构造参数 `size_type vertex_cnt`​ ，表示本图的实际点数。

2. 时间复杂度

   $O(\frac{n^2} \omega)$ ，此处 `w` 指掩膜类型的位数。

3. 备注

   `Bron-Kerbosch` 算法处理的问题为无向图中的最大团、最大独立集、最小顶点覆盖问题。

   最大团表示，每一对点之间都存在直接边的最大点集。
   
   最大独立集表示，每一对点之间，都不存在直接边的最大点集。

   最小顶点覆盖表示，选出最少数量的顶点点集，使得原图的每条边上都有端点在点集中。
   
   显然，最大团和最大独立集是镜像问题，求某图的最大团相当于求该图补图的最大独立集。
   
   而最大独立集和最小顶点覆盖为等价问题，在同一张图里，最大独立集的补集恰为最小覆盖点集，最小覆盖点集的补集恰为最大独立集。
   
   由于本模板只处理较小的图，且用邻接矩阵存图，所以会忽略掉重边。
   
   本模板会忽略掉自环。
   

#### 2.加边(add_edge)

1. 数据类型

   输入参数 `size_type a`​ ，表示无向边的起点编号。

   输入参数 `size_type b` ，表示无向边的终点编号。

2. 时间复杂度

   $O(1)$ 。


#### 3.计算最大团大小(max_clique)

1. 数据类型

   返回类型 `size_type` ，表示最大团的大小。

2. 时间复杂度

   很难计算最坏时间复杂度，一般对 `100~200` 范围内的点数、边数运行很快。

#### 4.计算最大独立集大小(max_independent_set)

1. 数据类型

   返回类型 `size_type` ，表示最大独立集的大小。

2. 时间复杂度

   很难计算最坏时间复杂度，一般对 `100~200` 范围内的点数、边数运行很快。

#### 5.找到最大团(get_max_clique)

1. 数据类型

   返回类型 `std::vector<size_type>` ，表示最大团方案。

2. 时间复杂度

   很难计算最坏时间复杂度，一般对 `100~200` 范围内的点数、边数运行很快。

3. 备注

   本方法找到的最大团保证为字典序最小的最大团。

#### 6.找到最大独立集(get_max_independant_set)

1. 数据类型

   返回类型 `std::vector<size_type>` ，表示最大独立集方案。

2. 时间复杂度

   很难计算最坏时间复杂度，一般对 `100~200` 范围内的点数、边数运行很快。

3. 备注

   本方法找到的最大独立集保证为字典序最小的最大独立集。

#### 7.对所有团调用回调(do_for_each_clique)

1. 数据类型

   输入参数 `Callback &&call` ，表示要对团调用的回填函数。

2. 时间复杂度

    $O(2^\sqrt {2\cdot m})$ ，此处 `m` 指去重后的边数。

3. 备注

   回调函数 `call` 需要传入一个 `std::vector<size_type>` 类型的参数，这个 `vector` 存放的是团里的结点编号。

#### 8.对所有独立集调用回调(do_for_each_independant_set)

1. 数据类型

   输入参数 `Callback &&call` ，表示要对独立集调用的回填函数。

2. 时间复杂度

    $O(2^\sqrt {2\cdot m})$ ，此处 `m` 指在补图里的无重边数。

3. 备注

   回调函数 `call` 需要传入一个 `std::vector<size_type>` 类型，这个 `vector` 存放的是独立集里的结点编号。

### 三、模板示例

```c++
#include "GRAPH/BronKerbosch.h"
#include "IO/FastIO.h"

int main() {
    // 无向图
    OY::BK::Graph<uint64_t, 100> G(5);
    // 加边
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(0, 2);
    G.add_edge(3, 1);
    // 求最大团
    cout << "max clique size: " << G.max_clique() << '\n';
    // 求最大独立集
    cout << "max independant set size: " << G.max_independant_set() << '\n';
    // 求字典序最小的最大团方案
    auto max_clique = G.get_max_clique();
    cout << "max clique:\n";
    for (int i = 0; i < max_clique.size(); i++) cout << max_clique[i] << " \n"[i == max_clique.size() - 1];
    // 求字典序最小的最大独立集方案
    auto max_independant_set = G.get_max_independant_set();
    cout << "max independant set:\n";
    for (int i = 0; i < max_independant_set.size(); i++) cout << max_independant_set[i] << " \n"[i == max_independant_set.size() - 1];
    // 遍历所有的团
    cout << "cliques:\n";
    G.enumerate_clique([](const std::vector<uint32_t> &clique) {
        for (int i = 0; i < clique.size(); i++) cout << clique[i] << " \n"[i == clique.size() - 1];
    });
    // 遍历所有的独立集
    cout << "independant sets:\n";
    G.enumerate_independant_set([](const std::vector<uint32_t> &independant_set) {
        for (int i = 0; i < independant_set.size(); i++) cout << independant_set[i] << " \n"[i == independant_set.size() - 1];
    });
}
```

```
#输出如下
max clique size: 3
max independant set size: 3
max clique:
0 1 2
max independant set:
0 3 4
cliques:
0
0 1
0 1 2
0 2
1
1 2
1 3
2
3
4
independant sets:
0
0 3
0 3 4
0 4
1
1 4
2
2 3
2 3 4
2 4
3
3 4
4

```

