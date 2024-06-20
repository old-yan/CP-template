### 一、模板类别

​	数据结构：`TwoSat` 算法。

​	练习题目：

1. [P4782 【模板】2-SAT](https://www.luogu.com.cn/problem/P4782)
2. [2 Sat](https://judge.yosupo.jp/problem/two_sat)(https://github.com/yosupo06/library-checker-problems/issues/26)

### 二、模板功能

#### 1.构造图

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示图中编号的类型。

   构造参数 `size_type vertex_cnt` 。

   构造参数 `size_type clause_cnt` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   `Twosat` 算法处理的问题为适定性问题。给出一堆变量，这些变量的值可以取 `0` 或者取 `1` ，而且它们之间存在一些约束关系，问是否有可能的方案。那么这个问题可以使用本数据结构解决。

   本数据结构可以接受重边和自环。


#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type vertex_cnt` ，表示点数。

   输入参数 `size_type clause_cnt` ，表示断言数。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法会强制清空之前的数据，并建立新图。

#### 3.加约束(add_clause)

1. 数据类型

   输入参数 `size_type i`​ ，表示某个变量编号。

   输入参数 `bool i_val` ，表示 `i` 的值。

   输入参数 `size_type j` ，表示某个变量编号。

   输入参数 `bool j_val` ，表示 `j` 的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   这条约束并不是指 `i` 的值必须为 `i_val` ，且 `j` 的值必须为 `j_val` 。而是指 `i` 的值为 `i_val` ，或 `j` 的值为 `j_val` ，这两个命题中至少有一条成立即可。


#### 4.计算可能性(calc)

1. 数据类型

   返回类型 `bool` ，表示最终可能性。

2. 时间复杂度

   $O(n + m)$ 。

3. 备注

   若有可行方案，返回 `true` 且存储了每个变量的值。

#### 5.查询方案里某点的状态(query)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的结点。

   返回类型 `bool` ，表示在所得方案里结点为真为假。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   方案可以是多样化的，本方法只会返回上次调用 `calc` 所得的那种方案。

### 三、模板示例

```c++
#include "GRAPH/Twosat.h"
#include "IO/FastIO.h"

int main() {
    // 六个变量，五条约束
    OY::TWOSAT::Graph G(6, 5);
    // 加五条约束
    G.add_clause(0, true, 1, true);
    G.add_clause(0, true, 2, false);
    G.add_clause(0, false, 3, true);
    G.add_clause(1, false, 5, false);
    G.add_clause(2, false, 3, true);
    // 计算可能性
    if (!G.calc()) {
        cout << "It's impossible\n";
    } else {
        cout << "It's possible\n";
        for (int i = 0; i < 6; i++) {
            cout << i << "'s value:" << (G.query(i) ? "true\n" : "false\n");
        }
    }
}
```

```
#输出如下
It's possible
0's value:false
1's value:true
2's value:false
3's value:true
4's value:false
5's value:false

```

