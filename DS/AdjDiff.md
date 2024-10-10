### 一、模板类别

​	数据结构：差分。

​	练习题目：

1. [Minimum Sum](https://acm.hdu.edu.cn/showproblem.php?pid=3473)
2. [Best Reward](https://acm.hdu.edu.cn/showproblem.php?pid=3613)
3. [1310. 子数组异或查询](https://leetcode.cn/problems/xor-queries-of-a-subarray/)
4. [P1117 [NOI2016] 优秀的拆分](https://www.luogu.com.cn/problem/P1117)
5. [P2367 语文成绩](https://www.luogu.com.cn/problem/P2367)
6. [P4655 [CEOI2017] Building Bridges](https://www.luogu.com.cn/problem/P4655)
7. [P10843 【MX-J2-T4】Turtle and Cycles](https://www.luogu.com.cn/problem/P10843)
8. [fsl 的背包](https://ac.nowcoder.com/acm/problem/263978)
9. [Static Range Sum](https://judge.yosupo.jp/problem/static_range_sum)(https://github.com/yosupo06/library-checker-problems/issues/398)


### 二、模板功能

#### 1.初始化

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示数组大小、编号的类型。

   模板参数 `typename CommutativeGroup` ，表示交换群类型。

   模板参数 `bool AutoSwitch` ，表示是否自动切换状态。

   构造参数 `size_type length`  ，表示维护的区间下标范围为 `[0, length-1]` 。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   本模板通过模板参数 `typename CommutativeGroup` 确定交换群。交换群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

4. 定义静态函数 `inverse` ，输入参数一个 `value_type` ，返回其逆元。

    本模板要求区间操作函数的运算符满足**结合律**和**交换律**。常见的交换群为加法群和异或群。

   本数据结构，处于三种状态之一，且可以随时切换。

   当处于差分态 `TABLE_DIFFERENCE` 时，便于进行“将某个子数组增加一定的值”操作；

   当处于值态时 `TABLE_VALUE` ，便于进行“将某个点增加一定的值”或者“查询某点的值”操作；

   当处于前缀和态 `TABLE_PRESUM` 时，便于进行“查询某个子数组的和”操作。

   状态切换需要 $O(n)$ 的时间，所以使用时尽量避免频繁切换状态。

   如果担心忘记切换状态，可以将 `AutoSwitch` 设为 `true` ，则在调用函数时会自动切换到最合适的状态。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。如果不传递此参数，初状态为 `TABLE_ANY` 态。如果进行了有意义的初始化，则初状态为 `TABLE_VALUE` 态。

#### 2.初始化

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到表里。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length`  ，表示维护的行范围为 `[0, length-1]` 。

   输入参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到差分表里。

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。在调用时，会按照行下标从 `0` 到 `length-1` 依次调用。
   
   本函数没有进行参数检查，所以请自己确保下标合法。（位于 `[0，n)`）

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到表里。

   
#### 5.单点增值(add)

1. 数据类型

   输入参数 `size_type i` ，表示要增值的点所在下标。

   输入参数 `value_type inc`​ ，表示要增加的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在值态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（行号位于 `[0，n)`）

#### 6.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i` ，表示要赋值的点所在下标。

   输入参数 `value_type inc`​ ，表示要赋的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在值态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（行号位于 `[0，n)`）

#### 7.区间增值(add)

1. 数据类型

   输入参数 `size_type left` ，表示要增值的区间的开始下标。

   输入参数 `size_type right` ，表示要增值的区间的结尾下标。（闭区间）

   输入参数 `value_type inc`​ ，表示要增加的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在差分态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（行号位于 `[0，n)`）

#### 8.单点查询(query)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的点所在下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在值态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（行号位于 `[0，n)`）

#### 9.区间查询(query)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的子数组的开始下标。

   输入参数 `size_type right` ，表示要查询的子矩形的结尾下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在前缀和态下进行。

   本函数没有进行参数检查，所以请自己确保下标合法。（行号位于 `[0，n)`）

#### 10.查询全部(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本操作须在前缀和态下进行。


#### 11.切换状态到差分态(switch_to_difference)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到差分态，以便进行区间修改。

#### 12.切换状态到值态(switch_to_value)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到值态，以便进行单点修改和单点查询。

   **注意：**如果要进行整表输出，也请切换到值态。

#### 13.切换状态到前缀和态(switch_to_presum)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

3. 备注

   切换到前缀和态，以便进行区间查询。


### 三、模板示例

```c++
#include "DS/AdjDiff.h"
#include "IO/FastIO.h"

int main() {
    // 当 AutoSwitch 为 true 时，其实可以不用手动转换状态
    OY::AdjSumTable<int, true> ad(5);

    cout << ad << endl;

    // ad.switch_to_value();
    ad.add(1, 10);
    ad.add(2, 100);
    cout << ad << endl;

    // ad.switch_to_difference();
    ad.add(2, 4, 1000);

    // ad.switch_to_value();
    cout << ad << endl;
}
```

```
#输出如下
[0, 0, 0, 0, 0]
[0, 10, 100, 0, 0]
[0, 10, 1100, 1000, 1000]

```

