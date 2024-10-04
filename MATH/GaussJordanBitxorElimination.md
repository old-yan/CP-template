### 一、模板类别

​	数学：高斯-约旦异或消元法。

​	练习题目：

1. [P2447 [SDOI2010] 外星千足虫](https://www.luogu.com.cn/problem/P2447)
2. [System of Linear Equations (Mod 2)](https://judge.yosupo.jp/problem/system_of_linear_equations_mod_2)(https://github.com/yosupo06/library-checker-problems/issues/896)


### 二、模板功能

#### 1.分式求某项系数

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示模板中涉及的数字类型。
   
   类型设定 `bitset_type = std::bitset<MAX_UNKNOWN + 1>` ，表示模板中用以表示方程、向量的类型。借助 `std::bitset` ，可以大幅加速运算。
   
   模板参数 `typename Tp` ，表示元素类型。

   模板参数 `size_type MAX_UNKNOWN` ，表示未知数的最大数量。

   模板参数 `size_type MAX_EQUATION` ，表示方程的最大数量。

   构造参数 `size_type unknown` ，表示未知数的具体数量。

   构造参数 `size_type equation` ，表示方程的具体数量。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   本高斯消元解决的是，给定一组异或方程，求解一组未知数的问题。

   方程的形式为，等号左侧为所有的未知数与其系数的异或值的异或和；等号右侧为结果值。

   在异或问题中，元素类型固定为 `bool` ，且可以使用 `std::bitset` 来进行加速，所以有特化的异或高斯消元模板来解决。

#### 2.获取某方程的某项系数(coef)

1. 数据类型

   输入参数 `size_type equation_ID` ，表示要设置的方程的编号。

   输入参数 `size_type unknown_ID` ，表示要设置的未知数的编号。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本方法有只读和可写两个重载。


#### 3.获取某方程的右侧值(result)

1. 数据类型

   输入参数 `size_type equation_ID` ，表示要设置的方程的编号。

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本方法有只读和可写两个重载。

#### 4.设置某方程(set_equation)

1. 数据类型

   输入参数 `size_type equation_ID` ，表示要设置的方程的编号。

   输入参数 `const bitset_type &equation` ，表示要设置的方程。

2. 时间复杂度

   $O(\frac n w)$ ，此处 `n` 指未知数数量， `w` 指 `std::bitset` 运算时的字长。

3. 备注

   本方法使用 `bitset_type` 一次性设置整个方程。

#### 5.计算(calc)

1. 数据类型

   返回类型 `bool` ，表示方程组是否有解。

2. 时间复杂度

    $O(\frac {n^3} w)$ 。

3. 备注

   在调用本方法前，请先将所有方程的未知数系数和右侧值填写好。

   当方程组无解时，返回 `false` 。

   当方程组有唯一解或者多解时，返回 `true` 。

#### 6.查询是否有多解(has_multi_solution)

1. 数据类型

   返回类型 `bool` ，表示查询结果。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在调用本方法前，须先调用计算。

#### 7.获取某未知数的解(get_solution)

1. 数据类型

   输入参数 `size_type unknown_ID` ，表示要查询的未知数的编号。

   返回类型 `Tp` ，表示该未知数的值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在调用本方法前，须先调用计算。

#### 8.获取解系的秩(rank)

1. 数据类型

   返回类型 `size_type` 。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   在调用本方法前，须先调用计算。
   
   通过 `get_solution` 只能获得一个解，可以在其基础上添加任意个特定的基向量，仍然是原方程的解。
   
   本方法返回的就是，最大线性无关基向量数目。通过 `enumerate_base` 可以访问这些基向量。

#### 9.枚举基向量(enumerate_base)

1. 数据类型

   输入参数 `Callback &&call` ，表示回调函数。

2. 时间复杂度

   $O(m*n)$ ，其中 `m` 表示未知数数量， `n` 表示方程数量。

3. 备注

   在调用本方法前，须先调用计算。
   


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/GaussJordanBitxorElimination.h"
#include "MATH/StaticModInt32.h"

int main() {
    // 异或高斯消元
    OY::GJBE::GaussJordanBitxorElimination<10, 10> GE(3, 1);
    // 0 x + 0 y + 0 z = 1
    GE.set_equation(0, std::bitset<11>{"1000"});
    // 计算
    if (!GE.calc()) {
        cout << "No Solution\n";
    } else if (GE.has_multi_solution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE.get_solution(i) << endl;
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE.get_solution(i) << endl;
    }

    // 异或高斯消元
    OY::GJBE::GaussJordanBitxorElimination<10, 10> GE2(3, 1);
    // 1 x + 1 y + 1 z = 1
    GE2.set_equation(0, std::bitset<11>{"1111"});
    // 计算
    if (!GE2.calc()) {
        cout << "No Solution\n";
    } else if (GE2.has_multi_solution()) {
        cout << "Multi Solution. Possible solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE2.get_solution(i) << endl;
        cout << "Rank of solution: " << GE2.rank() << endl;
        // 一组基 x + y = 0，说明可以给解的 x 和 y 同时做修改，修改后仍为一组解
        // 一组基 x + z = 0，说明可以给解的 x 和 z 同时做修改，修改后仍为一组解
        GE2.enumerate_base([](decltype(GE2)::bitset_type x) {
            cout << x.to_string().substr(7) << endl;
        });
    } else {
        cout << "Unique Solution:\n";
        for (int i = 0; i < 3; i++)
            cout << "x" << i << " = " << GE2.get_solution(i) << endl;
    }
}
```

```
#输出如下
No Solution
Multi Solution. Possible solution:
x0 = 1
x1 = 0
x2 = 0
Rank of solution: 2
0011
0101

```

