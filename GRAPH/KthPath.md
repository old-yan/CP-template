### 一、模板类别

​	数据结构：`K` 短路。

​	练习题目：

1. [P2483 【模板】k 短路 / [SDOI2010] 魔法猪学院](https://www.luogu.com.cn/problem/P2483)
2. [K-Shortest Walk](https://judge.yosupo.jp/problem/k_shortest_walk)(https://github.com/yosupo06/library-checker-problems/issues/509)

### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename Tp` ，表示边权类型。
   
   模板参数 `typename SumType` ，表示路径长度的类型。

   模板参数 `bool PassBy` ，表示路径能否多次路过终点。

   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。
   
   构造参数 `size_type edge_cnt` ，表示边数。无向边按两条边计。默认为 `0` 。
   
2. 时间复杂度

   $O(n+m)$ 。

3. 备注

    `K` 短路算法处理的问题为第 `k` 短的路有多长的问题。

   本数据结构要求边权为正。
   
   本数据结构可以接受重边和自环。


#### 2.加边(add_edge)

1. 数据类型

   输入参数 `size_type from`​ ，表示有向边的起点编号。

   输入参数 `size_type to` ，表示有向边的终点编号。

   输入参数 `Tp dis` ，表示有向边的边权。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   **注意：**在无向图中，需要按正反两个反向进行加边。

#### 3.计算可达性(calc)

1. 数据类型

   输入参数 `size_type source` ，表示起点编号。

   输入参数 `size_type target` ，表示终点编号。

   输入参数 `const SumType &infinite` ，表示无穷大边权。默认为 `SumType` 类的最大值的一半。

   返回类型 `bool` ，表示能否从起点到达终点。

2. 时间复杂度

   $O((m+n)\cdot\log n)$ 。

3. 备注

   调用本方法后，才可以调用 `next` 不断产生路径。

#### 4.获取下一条路径(next)

1. 数据类型

   输入参数 `const SumType &infinite` ，表示无穷大边权。默认为 `SumType` 类的最大值的一半。

   返回类型 `SumType` ，表示下一条最短路的总长度。

2. 时间复杂度

   $O(\log k)$ 。
   
3. 备注

   不断调用本方法即可依次求得前 `k` 短路径的长度。

   本方法可以多次调用，显然，随着本函数的调用次数增加，返回值单调不减。

   如果已经枚举完所有的可行路线，接下来的返回值均为 `infinite` 值。

### 三、模板示例

```c++
#include "GRAPH/KthPath.h"
#include "IO/FastIO.h"

void test_directed() {
    // 有向图在不带环的情况下，路径数是有限的
    OY::KPATH::Graph<long long, long long, true> G(7, 9);
    G.add_edge(0, 1, 1);
    G.add_edge(1, 2, 1);
    G.add_edge(2, 3, 1);
    G.add_edge(3, 4, 1);
    G.add_edge(4, 5, 1);
    G.add_edge(5, 6, 1);
    G.add_edge(3, 4, 210);
    G.add_edge(4, 5, 200);
    G.add_edge(4, 5, 220);

    if (!G.calc(0, 6))
        cout << "can't go from 0 to 6\n";
    else
        for (int i = 0;; i++) {
            long long length = G.next();
            if (length > 9999999) break;
            cout << "No." << i << " path length: " << length << endl;
        }
}

void test_undirected() {
    // 如果图中出现了环，就有可能有无穷多的路径
    // 无向图本质上都是带环的有向图
    OY::KPATH::Graph<long long, long long, true> G(2, 2);
    G.add_edge(0, 1, 10);
    G.add_edge(1, 0, 3);

    if (!G.calc(0, 1))
        cout << "can't go from 0 to 1\n";
    else
        for (int i = 0; i < 10; i++) {
            long long length = G.next();
            cout << "No." << i << " path length: " << length << endl;
        }
}

int main() {
    test_directed();
    test_undirected();
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
No.0 path length: 10
No.1 path length: 23
No.2 path length: 36
No.3 path length: 49
No.4 path length: 62
No.5 path length: 75
No.6 path length: 88
No.7 path length: 101
No.8 path length: 114
No.9 path length: 127

```

