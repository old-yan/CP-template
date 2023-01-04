### 一、模板类别

​	数学：高斯-约旦消元法。

### 二、模板功能

#### 1.分式求某项系数

1. 数据类型

   模板参数 `typename _Tp` ，表示元素类型。

   模板参数 `uint32_t _MAXN` ，表示未知数和方程的最大数量。

   输入参数 `uint32_t __unknow` ，表示未知数的具体数量。

   输入参数 `uint32_t __equation` ，表示方程的具体数量。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   高斯消元解决的是，给定一组方程，求解一组未知数的问题。

   一般而言，方程组中方程数目和未知数数目在同一数量级。

   方程的形式为，等号左侧为所有的未知数与其系数的乘积之和；等号右侧为结果值。
   
   在异或问题中，元素类型固定为 `bool` ，且可以使用 `std::bitset` 来进行加速，所以有特化的异或高斯消元模板来解决。

#### 2.设置某方程的某项系数

1. 数据类型

   输入参数 `uint32_t __equationId` ，表示要设置的方程的编号。

   输入参数 `uint32_t __unknowId` ，表示要设置的未知数的编号。

2. 时间复杂度

   $O(1)$ 。


#### 3.设置某方程的右侧值

1. 数据类型

   输入参数 `uint32_t __equationId` ，表示要设置的方程的编号。

   输入参数 `_Tp __result` ，表示要设置的方程的右侧值。

2. 时间复杂度

   $O(1)$ 。

#### 4.设置某方程

1. 数据类型

   输入参数 `uint32_t __equationId` ，表示要设置的方程的编号。

   输入参数 `std::initializer_list<_Tp>& __equation` ，表示要设置的方程。

2. 时间复杂度

   $O(n)$ ，此处 `n` 指未知数数量。

3. 备注

   本方法仅用于普通高斯消元模板里，使用 `std::initializer_list<_Tp>` 一次性设置整个方程。

   **注意：** 请保证传入的方程长度为未知数数量加一。

#### 5.设置某方程

1. 数据类型

   输入参数 `uint32_t __equationId` ，表示要设置的方程的编号。

   输入参数 `const std::bitset<_MAXN+1>& __equation` ，表示要设置的方程。

2. 时间复杂度

   $O(\frac n w)$ ，此处 `n` 指未知数数量， `w` 指 `std::bitset` 运算时的字长。

3. 备注

   本方法仅用于异或高斯消元模板里，使用 `std::bitset` 一次性设置整个方程。

#### 6.计算

1. 数据类型

   输入参数 `_IsZero __isZero` ，表示判断元素为零的函数。

   返回类型 `bool` ，表示方程组是否有解。

2. 时间复杂度

   普通的高斯消元为 $O(n^3)$ ；使用 `bitset` 加速的异或高斯消元为 $O(\frac {n^3} w)$ 。

3. 备注

   在调用本方法前，请先将所有方程的未知数系数和右侧值填写好。

   如果 `_Tp` 为浮点数，由于浮点数存在精度误差，所以可能需要提供特殊的判零函数接口。一般而言， `__isZero` 函数采用默认参数即可。

   当方程组无解时，返回 `false` 。

   当方程组有唯一解或者多解时，返回 `true` 。

#### 7.查询是否有多解

1. 数据类型

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在调用本方法前，须先调用计算。

#### 8.获取某未知数的解

1. 数据类型

   输入参数 `uint32_t __unknowId` ，表示要查询的未知数的编号。

   返回类型 `_Tp` ，表示该未知数的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在调用本方法前，须先调用计算。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/GaussJordanElimination.h"
#include "MATH/StaticModInt.h"

int main() {
    // double 版本高斯消元，三个未知数，三个方程
    OY::GaussJordanElimination<double, 10> ge(3, 3);
    // 1 x + 3 y + 4 z = 5
    ge.setEquation(0, {1, 3, 4, 5});
    // 1 x + 4 y + 7 z = 3
    ge.setEquation(1, {1, 4, 7, 3});
    // 9 x + 3 y + 2 z = 2
    ge.setEquation(2, {9, 3, 2, 2});
    // 计算
    if (!ge.calc([](double x) { return std::abs(x) < 1e-9; })) {
        cout << "No Solution\n";
    } else if (ge.hasMultiSolution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << ge.getX(i) << endl;
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << ge.getX(i) << endl;
    }

    // 也可以适用于自取模数类
    // mint 版本高斯消元，两个未知数，四个方程
    using mint = OY::StaticModInt32<998244353, true>;
    OY::GaussJordanElimination<mint, 10> ge2(2, 3);
    // 1 x + 3 y = 5
    ge2.setEquation(0, {1, 3, 5});
    // 2 x + 6 y = 10
    ge2.setEquation(1, {2, 6, 10});
    // 0 x + 0 y = 0
    ge2.setEquation(2, {0, 0, 0});
    // 计算
    if (!ge2.calc()) {
        cout << "No Solution\n";
    } else if (ge2.hasMultiSolution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 2; i++)
            cout << "x" << i << " = " << ge2.getX(i) << endl;
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 2; i++)
            cout << "x" << i << " = " << ge2.getX(i) << endl;
    }

    // 异或高斯消元
    OY::GaussJordanXorElimination<10> ge3(3, 1);
    // 0 x + 0 y + 0 z = 1
    ge3.setCoef(0, 0, 0);
    ge3.setCoef(0, 1, 0);
    ge3.setCoef(0, 2, 0);
    ge3.setResult(0, 1);
    // 计算
    if (!ge3.calc()) {
        cout << "No Solution\n";
    } else if (ge3.hasMultiSolution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << ge3.getX(i) << endl;
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << ge3.getX(i) << endl;
    }
}
```

```
#输出如下
Unique Solution:
x0 = -0.973684
x1 = 5.184211
x2 = -2.394737
Multi Solution. Possible solution:
x0 = 5
x1 = 0
No Solution

```

