### 一、模板类别

​	数据结构：数字集

​	练习题目：

1. [Hotaru's problem](https://acm.hdu.edu.cn/showproblem.php?pid=5371)
2. [P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)
3. [U156719 Dynamic Predecessor Problem](https://www.luogu.com.cn/problem/U156719)
4. [Predecessor Problem](https://judge.yosupo.jp/problem/predecessor_problem)(https://github.com/yosupo06/library-checker-problems/issues/632)

### 二、模板功能

#### 1.建立数字集

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示位集下标类型。

   模板参数 `size_t MAX_LEVEL` ，表示位集的大小为 `1<<MAX_LEVEL`。

2. 时间复杂度

   $O(\frac n \omega )$ ，其中 `ω` 表示 `MaskType` 所包含的比特数量。

3. 备注

   本模板与 `OY::Bitset` 的区别是，本模板没有区间 `set`、区间 `reset`、区间 `flip` 等方法。本模板主要的区间操作，只有全体 `set`，全体 `reset`，以及全体 `count` 。而本模板的速度最快的操作就是寻找前驱、后继；对于给定值域的整数集，维护元素增删、元素前驱后继的速度，本模板遥遥领先。

   本模板维护的值域大小一定是 `64` 的整数幂。例如，不论传递的 `L` 是 `13` 还是 `18` ，最终都会按 `18` 来确定数字集的大小，为 `1 << 18` 。

#### 2.将某一位置为 1(set)

1. 数据类型

   输入参数 `size_type pos` ，表示将下标 `pos` 位置置为 `1` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0, n)`）

#### 3.将某一位置为 0(reset)

1. 数据类型

   输入参数 `size_type pos` ，表示将下标 `pos` 位置置为 `0` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0, n)`）

####  4.全部置为1(set)

1. 数据类型

2. 时间复杂度

   $O(\frac n \omega)$ 。

3. 备注

   调用本函数会将位集的每一位置为 `1` 。

####  5.全部置为 0(reset)

1. 数据类型

2. 时间复杂度

   $O(\frac n \omega)$ 。

3. 备注

   调用本函数会将位集的每一位置为 `0` 。

#### 6.查询位集是否为空(empty)

1. 数据类型

   返回类型 `bool` ，表示数字集是否为空。

2. 时间复杂度

   $O(1)$ 。

#### 7.查询1的总数(count)

1. 数据类型

   返回类型 `size_type` ，表示数字集中 `1` 的数量。

2. 时间复杂度

   $O(\frac n \omega)$ 。

#### 8.查询数字集大小(size)

1. 数据类型

   返回类型 `size_type` ，表示数字集维护的值域区间长度。

2. 时间复杂度

   $O(1)$ 。

#### 9.查询某下标(at)

1. 数据类型

   输入参数 `size_type pos` ，表示要查询的下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0, n)`）

#### 10.查询前一个1的位置(smaller_bound)

1. 数据类型

   输入参数 `size_type pos` ，表示当前下标。

2. 时间复杂度

   $O(\log n)$ ，此处底数为 `64` 。

3. 备注

   当位集中下标 `pos` 之前全部都是 `0` ，没有 `1` 时，返回 `-1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0, n)`）

#### 11.查询后一个1的位置(lower_bound)

1. 数据类型

   输入参数 `size_type pos` ，表示当前下标。

2. 时间复杂度

   $O(\log n)$ ，此处底数为 `64` 。

3. 备注

   当位集中下标 `pos` 及之后全部都是 `0` ，没有 `1` 时，返回 `-1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0, n)`）

#### 12.查询后一个1的位置(upper_bound)

1. 数据类型

   输入参数 `size_type pos` ，表示当前下标。

2. 时间复杂度

   $O(\log n)$ ，此处底数为 `64` 。

3. 备注

   当位集中下标 `pos` 之后全部都是 `0` ，没有 `1` 时，返回 `-1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0, n)`）

#### 三、模板示例

```c++
#include "DS/IntegerSet.h"
#include "IO/FastIO.h"

int main() {
    OY::INTSET::Table<10> B;
    // 全部置为 1
    B.set();
    cout << "B.count() = " << B.count() << endl;
    for (int i = 139; i < 405; i += 50)
        cout << "index " << i << ":" << B.at(i) << endl;
    // 全部置为 0
    B.reset();
    cout << "B.count() = " << B.count() << endl;
    for (int i = 139; i < 405; i += 50)
        cout << "index " << i << ":" << B.at(i) << endl;

    B.set(16);
    B.set(980);
    B.set(357);
    B.set(2);
    for (int i = B.lower_bound(0); i >= 0; i = B.lower_bound(i + 1))
        cout << i << ' ';
    cout << endl;
    for (int i = B.smaller_bound(B.size()); i >= 0; i = B.smaller_bound(i))
        cout << i << ' ';
    cout << endl;
}
```

```
#输出如下
B.count() = 4096
index 139:1
index 189:1
index 239:1
index 289:1
index 339:1
index 389:1
B.count() = 0
index 139:0
index 189:0
index 239:0
index 289:0
index 339:0
index 389:0
2 16 357 980 
980 357 16 2 

```

