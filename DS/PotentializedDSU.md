### 一、模板类别

​	数据结构：权值并查集，维护势能关系的并查集

​	练习题目：

1. [How Many Answers Are Wrong](https://acm.hdu.edu.cn/showproblem.php?pid=3038)
2. [P1196 [NOI2002] 银河英雄传说](https://www.luogu.com.cn/problem/P1196)
3. [P2024 [NOI2001] 食物链](https://www.luogu.com.cn/problem/P2024)
4. [Unionfind with Potential](https://judge.yosupo.jp/problem/unionfind_with_potential)(https://github.com/yosupo06/library-checker-problems/issues/1194)


### 二、模板功能

​		本模板为 `DSU` 在维护群信息情况下的扩展。

​		本模板要求提供模板参数 `typename Group` 群类型。群中规定了元素类型、幺元的值、元素结合操作、元素的逆元。

​		为了方便使用，本模板提供了 `AddPDSUTable` 作为维护加法群的权值并查集； `ModAddPDSUTable` 作为维护同余环内加法群的权值并查集。基于此，可以不使用 `modint` 解决相关问题。

#### 1.查询分组首领(find)

1. 数据类型

   参数 `size_type i` ，表示要寻找元素 `i` 的信息。
   
   返回类型 `info` ，包含两个成员变量。 `m_head` 成员表示元素 `i` 所在分组的首领； `m_val` 成员表示元素 `i` 到分组首领的势能差。

2. 时间复杂度

   均摊 $O(\alpha (n))$  。
   
3. 备注

    `m_val` 可以理解为，元素 `i` 的势能函数为 `f(i)` ；元素 `i` 所在分组的首领为 `j` ，元素 `j` 的势能函数为 `f(j)` 。则 `m_val` 为 `f(j) - f(i)` ，或者说 `i` 到 `j` 的距离。


#### 2.将前者合并到后者(unite_to)

1. 数据类型

   输入参数 `size_type head_a`  和 `size_type head_b` ，分别为各自分组的首领。
   
   输入参数 `value_type dis` ，表示 `head_a` 到 `head_b` 的距离为 `dis` 。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   表示将元素 `head_a`  所率领的分组合并到元素 `head_b` 所率领的分组。
   
   本函数的两个参数是有向的，不能随便交换。如果想在主代码中控制合并的方向，本函数是最好的选择。

#### 3.按照大小合并分组(unite_by_size)

1. 数据类型

   输入参数 `size_type a`  和 `size_type b` 。
   
   输入参数 `value_type dis` ，表示 `a` 到 `b` 的距离为 `dis` 。

   返回值 `bool`，表示是否合并成功。

2. 时间复杂度

   均摊 $O(\alpha (n))$  。
   
3. 备注

   表示将元素 `a` 和元素 `b` 所在的分组按照组的大小合并，将小组合并到大组上。

   如果 `a` 和 `b` 本来就在同一分组中，那么合并失败，本函数返回 `false` ；否则合并成功，返回 `true` 。

#### 4.按照ID合并分组(unite_by_ID)

1. 数据类型

   输入参数 `size_type a`  和 `size_type b` 。
   
   输入参数 `value_type dis` ，表示 `a` 到 `b` 的距离为 `dis` 。

   返回值 `bool`，表示是否合并成功。

2. 时间复杂度

   均摊 $O(\alpha (n))$  。
   
3. 备注

   表示将元素 `a` 和元素 `b` 所在的分组按照组长的 `ID` 大小合并，将 `ID` 较大的组合并到 ` ID` 较小的组上。

   如果 `a` 和 `b` 本来就在同一分组中，那么合并失败，本函数返回 `false` ；否则合并成功，返回 `true` 。

#### 5.查询二者关系(calc)

1. 数据类型：

   输入参数 `size_type a`  和 `size_type b` 。表示查询元素 `a` 和元素 `b` 。
   
   返回类型 `std::pair<bool, value_type>` ，表示查询结果。

2. 时间复杂度

   均摊 $O(\alpha (n))$  。

3. 备注

   当 `a` 和 `b` 不在同一个分组内时，返回 `{false, group::identity()}` 。
   
   当 `a` 和 `b` 在同一个分组内时，返回 `{true, dis(a, b)}` 。

### 三、模板示例

```c++
#include "DS/PotentializedDSU.h"
#include "IO/FastIO.h"

int main() {
    OY::AddPDSUTable<int, true> u(4);

    // 假设结点 i 处的势能函数为 f(i)
    // f(1) - f(0) = 100
    u.unite_by_size(0, 1, 100);

    // f(3) - f(2) = 40
    u.unite_by_size(2, 3, 40);

    // f(1) - f(2) = 10
    u.unite_by_size(2, 1, 10);

    // 此时，四个结点间的相互距离已经被确定
    cout << "dis from 0~2 = " << u.calc(0, 2).second << endl;
    cout << "dis from 2~1 = " << u.calc(2, 1).second << endl;
    cout << "dis from 1~3 = " << u.calc(1, 3).second << endl;
}
```

```
#输出如下
dis from 0~2 = 90
dis from 2~1 = 10
dis from 1~3 = 30

```

