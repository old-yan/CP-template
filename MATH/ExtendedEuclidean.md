### 一、模板类别

​	数学：扩展欧几里得算法。

​	练习题目：

1. [P1082 [NOIP2012 提高组] 同余方程](https://www.luogu.com.cn/problem/P1082)
2. [P5656 【模板】二元一次不定方程 (exgcd)](https://www.luogu.com.cn/problem/P5656)


### 二、模板功能

#### 1.求解最大公约数

1. 数据类型

   输入参数 `uint64_t a` ，表示第一个数，要求非负。

   输入参数 `uint64_t b` ，表示第二个数，要求非负。

   返回类型 `EuclideanTuple1` ，其 `m_gcd` 属性表示求出的最大公约数，`m_coef1` 属性和 `m_coef2` 属性表示配平数。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本模板基于欧几里得算法求解最大公约数，并给出 `a` 和 `b` 的配平数，使得 `a * m_coef1 + b * m_coef2 = m_gcd` 。这样的配平数可以存在无数多组，但只返回其中一组。
   特别的，当 `a` 为零或者 `b` 为零时，返回 `m_gcd = m_coef1 = m_coef2 = 0` 。

#### 2.求解配平式

1. 数据类型

   输入参数 `int64_t a` ，表示第一个数。

   输入参数 `int64_t b` ，表示第二个数。

   输入参数 `int64_t target` ，表示要求配平得到的目标值。
   
   输入参数 `int64_t coef_a_thresh` ，表示要求数 `a` 的系数的最小值。

   返回类型 `EuclideanTuple1` ，其 `m_gcd` 属性表示求出的最大公约数，`m_coef1` 属性表示配平数， `m_flag` 属性表示配平结果。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本模板基于欧几里得算法计算配平数，使得 `a * m_coef1 + b * m_coef2 = target` 。这样的配平数可以存在无数多组，返回的一组是令 `m_coef1 >= coef_a_thresh` 且 `m_coef1` 最小的一组。本算法只偏重计算合适的 `m_coef1` ，所以 `m_coef2` 很有可能超出一定的数值范围，所以没有返回 `m_coef2` 。
   
   默认 `coef_a_thresh` 为零，表示计算最小的非负 `m_coef1` 。

   当无法配平时，返回 `m_flag = false` ；否则返回 `m_flag = true` 。特别的，如果 `a` 和 `b` 均为零，无论是否能配平，都返回 `false` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/ExtendedEuclidean.h"

int main() {
    int a = 100;
    int b = -85;
    // 只找出最大公约数
    auto res = OY::ExtenedEuclideanSolver::solve(a, b);
    cout << "gcd of " << a << " and " << b << " is: " << res.m_gcd << endl;
    cout << a << " * " << res.m_coef1 << " + " << b << " * " << res.m_coef2 << " = " << res.m_gcd << endl;

    // 尝试配平 -20
    int c = -20;
    auto res2 = OY::ExtenedEuclideanSolver::solve(a, b, c);
    if (res2.m_flag) {
        auto coef1 = res2.m_coef1;
        auto coef2 = (c - coef1 * a) / b;
        cout << "gcd of " << a << " and " << b << " is: " << res2.m_gcd << endl;
        cout << a << " * " << coef1 << " + " << b << " * " << coef2 << " = " << c << endl;
    } else {
        cout << "failed\n";
    }
}
```

```
#输出如下
gcd of 100 and -85 is: 5
100 * 6 + -85 * 7 = 5
gcd of 100 and -85 is: 5
100 * 10 + -85 * 12 = -20

```

