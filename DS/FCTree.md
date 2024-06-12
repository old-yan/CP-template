### 一、模板类别

​	数据结构： `FC` 树。


### 二、模板功能

#### 1.建立FC树

1. 数据类型

   类型设定 `size_type = size_t` ，表示下标编号的变量类型。

   构造参数 `size_type length` ，表示 `FC` 树的覆盖范围为 `[0, length)`。
   
   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射。
   
   构造参数 `PermMapping perm_mapping` ，表示查询所依赖的排列的映射。

2. 时间复杂度

    $O(n)$ 。

3. 备注

    `FC` 树是自己起的模板名字。由于在实现中借助了 `fractional cascading` 的方法，所以称之为 `FC` 树。
    
    `FC` 树处理的问题为，给定一个长度为 `n` 的数组 `a` ，再给定一个 `0~n-1` 的排列 `perm` 。修改时，需要对 `a[l]` ， `a[l+1]` ，...， `a[r]` 进行区间加修改；查询时，对 `a[perm[l]]` ， `a[perm[l+1]]` ，...， `a[perm[r]]` 进行和值查询。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示 `FC` 树的覆盖范围为 `[0, length)`。
   
   输入参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射。
   
   输入参数 `PermMapping perm_mapping` ，表示查询所依赖的排列的映射。

2. 时间复杂度

    $O(n)$ 。
   
#### 3.区间增值(add)

1. 数据类型

   输入参数 `size_type left` ，表示区间增值的开头下标。

   输入参数 `size_type right​` ，表示区间增值的结尾下标。(闭区间)

   输入参数 `Tp inc` ，表示要进行增加的值。

2. 时间复杂度

   $O(\sqrt n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）

#### 4.单点查询(query)

1. 数据类型

   输入参数 `size_type pos` ，表示单点查询的下标。
   
   返回类型 `Tp` ，表示查询到的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 10.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)
   
   返回类型 `Tp` ，表示查询到的和值。

2. 时间复杂度

   $O(\sqrt n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）

### 三、模板示例

```c++
#include "DS/FCTree.h"
#include "IO/FastIO.h"

void test() {
    int perm[] = {4, 9, 2, 3, 5, 7, 8, 1, 6, 0};
    auto mapping = [](int i) { return i * 10; };
    auto perm_mapping = [&](int i) { return perm[i]; };
    
    OY::FCTree<int> S(10, mapping, perm_mapping);
    for (int i = 0; i < 10; i++) cout << S.query(i) << " \n"[i == 9];

    S.add(2, 6, 1000);
    for (int i = 0; i < 10; i++) cout << S.query(i) << " \n"[i == 9];
}

void brute_force() {
    int perm[] = {4, 9, 2, 3, 5, 7, 8, 1, 6, 0};
    auto mapping = [](int i) { return i * 10; };
    
    int S[10]{};
    for (int i = 0; i < 10; i++) S[i] = mapping(i);
    for (int i = 0; i < 10; i++) cout << S[perm[i]] << " \n"[i == 9];

    for (int i = 2; i <= 6; i++) S[i] += 1000;
    for (int i = 0; i < 10; i++) cout << S[perm[i]] << " \n"[i == 9];
}

int main() {
    test();
    brute_force();
}
```

```
#输出如下
40 90 20 30 50 70 80 10 60 0
1040 90 1020 1030 1050 70 80 10 1060 0
40 90 20 30 50 70 80 10 60 0
1040 90 1020 1030 1050 70 80 10 1060 0


```

