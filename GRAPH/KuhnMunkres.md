### 一、模板类别

​	数据结构： `Kuhn Munkres` 算法。

​	练习题目：

1. [P6577 【模板】二分图最大权完美匹配](https://www.luogu.com.cn/problem/P6577)
2. [#80. 二分图最大权匹配](https://uoj.ac/problem/80)
3. [Assignment Problem](https://judge.yosupo.jp/problem/assignment)(https://github.com/yosupo06/library-checker-problems/issues/38)


### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   模板参数 `typename _Tp` ，表示图中边权类型。
   
   构造参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   构造参数 `const Tp &init_value` ，表示初始边权，默认为 `0` 。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

    `KM` 算法处理的问题为二分图最大权完美匹配。

   本数据结构可以接受重边。

   本数据结构没有自环概念。

   设置初始边权用于解决不同类型问题。如果初始边权设为无穷小，则即使加入的边为负边，也会是相对来说权重较大的边，所以不会拒绝负的匹配；如果初始边权设为零，则加入的负边的权都不如默认边权，所以没有负匹配。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数，默认为 `0` 。

   输入参数 `const Tp &init_value` ，表示初始边权，默认为 `0` 。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加边(add_edge)

1. 数据类型

   输入参数 `size_type left`​ ，表示一条边的左侧结点编号。

   输入参数 `size_type right` ，表示有向边的右侧结点编号。

   输入参数 `Tp val` ，表示边权。

2. 时间复杂度

   $O(1)$ 。

#### 4.计算最大匹配(calc)

1. 数据类型

   返回类型 `Tp` ，表示求得的最大匹配值。

2. 时间复杂度

   $O(n^3)$ 。

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

#### 7.查询边权(query)

1. 数据类型

   输入参数 `size_type left` ，表示左侧结点编号。

   输入参数 `size_type right` ，表示右侧结点编号。

   返回类型 `Tp` ，表示 `left` 和 `right` 的匹配权值。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   若有重边，查询结果为连接两结点的最大边权；若无任何连边，返回默认边权。

### 三、模板示例

```c++
#include "GRAPH/KuhnMunkres.h"
#include "IO/FastIO.h"

void test() {
    cout << "prefect match:\n";
    // 建立二分图，求完美匹配
    OY::KM::Graph<int> G(3, -9999);
    // 加五条边，G.add_edge(a,b,value) 表示男孩 a 喜欢女孩 b
    G.add_edge(0, 0, 10);
    G.add_edge(1, 1, 100);
    G.add_edge(2, 2, 1000);
    G.add_edge(0, 1, 200);
    G.add_edge(1, 2, 2000);
    // 求最大权完美匹配
    cout << "max match = " << G.calc() << endl;
    // 看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "boy " << i << " matches girl " << int(G.find_right(i)) << endl;
    }
    // 看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        cout << "girl " << i << " matches boy " << int(G.find_left(i)) << endl;
    }
    cout << '\n';
}

void test2() {
    cout << "prefect match:\n";
    // 建立二分图，求最大匹配（不要求完美）
    OY::KM::Graph<int> G(3, 0);
    // 加五条边，G.add_edge(a,b,value) 表示男孩 a 喜欢女孩 b
    G.add_edge(0, 0, 10);
    G.add_edge(1, 1, 100);
    G.add_edge(2, 2, 1000);
    G.add_edge(0, 1, 200);
    G.add_edge(1, 2, 2000);
    // 求最大权完美匹配
    cout << "max match = " << G.calc() << endl;
    // 看每个男孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        if (G.query(i, G.find_right(i)) != 0)
            cout << "boy " << i << " matches girl " << int(G.find_right(i)) << endl;
    }
    // 看每个女孩的对象
    for (uint32_t i = 0; i < 3; i++) {
        if (G.query(G.find_left(i), i) != 0)
            cout << "girl " << i << " matches boy " << int(G.find_left(i)) << endl;
    }
    cout << '\n';
}

int main() {
    test();
    test2();
}
```

```
#输出如下
prefect match:
max match = 1110
boy 0 matches girl 0
boy 1 matches girl 1
boy 2 matches girl 2
girl 0 matches boy 0
girl 1 matches boy 1
girl 2 matches boy 2

prefect match:
max match = 2200
boy 0 matches girl 1
boy 1 matches girl 2
girl 1 matches boy 0
girl 2 matches boy 1

```

