### 一、模板类别

​	数据结构：`Twosat` 算法。

### 二、模板功能

#### 1.构造图

1. 数据类型

   模板参数 `typename _Solver` ，表示采用的解法。可以为 `Garbow` ，`Kosaraju` 或者 `Tarjan_scc` 中的一种，默认为 `Tarjan_scc` 。

   构造参数 `uint32_t __variableNum`​ ，表示变量的数目。

   构造参数 `uint32_t __clauseNum` ，表示约束语句的数目。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   `Twosat` 算法处理的问题为适定性问题。给出一堆变量，这些变量的值可以取 `0` 或者取 `1` ，而且它们之间存在一些约束关系，问是否有可能的方案。那么这个问题可以使用本数据结构解决。

   本数据结构可以接受重边和自环。

   **注意：**

   本数据结构一开始指定的 `__variableNum` 参数必须是确切值。

   一开始指定的 `__clauseNum` 参数可以是模糊值，是用来为加约束预留空间的，即使一开始没有留够空间，也可以自动扩容。如果约束数不确定，可以预估一个上限填进去。

#### 2.加约束

1. 数据类型

   输入参数 `uint32_t __i`​ ，表示某个变量编号。

   输入参数 `bool __iVal` ，表示 `__i` 的值。

   输入参数 `uint32_t __j` ，表示某个变量编号。

   输入参数 `bool __jVal` ，表示 `__j` 的值。

2. 时间复杂度

   $O(1)$。

3. 备注

   这条约束并不是指 `__i` 的值必须为 `__iVal` ，且 `__j` 的值必须为 `__jVal` 。而是指 `__i` 的值为 `__iVal` ，或 `__j` 的值为 `__jVal` ，这两个命题中至少有一条成立即可。


#### 3.计算可能性

1. 数据类型

   返回类型 `bool` ，表示最终可能性。

2. 时间复杂度

   $O(n+m)$ 。

3. 备注

   若有可行方案，返回 `true` 且 `m_choose` 中存储了每个变量的值。

### 三、模板示例

```c++
#include "GRAPH/Twosat.h"
#include "IO/FastIO.h"

int main() {
    //六个变量，五条约束
    OY::TwoSat<OY::Kosaraju> G(6, 5);
    //加五条约束
    G.addClause(0, true, 1, true);
    G.addClause(0, true, 2, false);
    G.addClause(0, false, 3, true);
    G.addClause(1, false, 5, false);
    G.addClause(2, false, 3, true);
    //计算可能性
    if (!G.calc()) {
        cout << "It's impossible\n";
    } else {
        cout << "It's possible\n";
        for (int i = 0; i < 6; i++) {
            cout << i << "'s value:" << (G.m_choose[i] ? "true\n" : "false\n");
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

