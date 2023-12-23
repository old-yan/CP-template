### 一、模板类别

​	数学：动态矩阵（长宽可变）。

​	练习题目：

1. [P3390 【模板】矩阵快速幂](https://www.luogu.com.cn/problem/P3390)

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename Tp` ，表示元素的类型。

   构造参数 `uint32_t m` ，表示行数。

   构造参数 `uint32_t n` ，表示列数。

   构造参数 `const std::valarray<std::valarray<Tp>> &vals` ，表示赖以赋值的矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   本模板可以使用花括号（初始化列表）进行构造。注意每行的元素数量要保持一致。

#### 2.构造

1. 数据类型

   模板参数 `typename Tp` ，表示元素的类型。

   构造参数 `uint32_t m` ，表示行数。

   构造参数 `uint32_t n` ，表示列数。

   构造参数 `const std::valarray<std::valarray<Tp>> &vals` ，表示赖以赋值的矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   本模板可以使用花括号（初始化列表）进行构造。注意每行的元素数量要保持一致。

#### 3.同值矩阵(raw)

1. 数据类型

   输入参数 `uint32_t m` ，表示行数。

   输入参数 `uint32_t n` ，表示列数。

   输入参数 `Tp a` ，表示矩阵中元素的值。默认为 `Tp` 类的无参实例。

2. 时间复杂度

   $O(m\times n)$ 。


#### 4.单位矩阵(unit)

1. 数据类型

   输入参数 `uint32_t m` ，表示方阵边长。

   返回类型 `DynamicMatrix<Tp>` ，为主对角线元素值均为 `1` 的矩阵。

2. 时间复杂度

   $O(m^2)$ 。

3. 备注

   除对角线外其余值为 `0` 。

#### 5.与元素进行二元运算

1. 数据类型

   输入参数 `Tp a` ，表示第二个操作数。

   返回类型 `StaticMatrix<Tp, M, N>` ，为结果矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   包含 `+=`，`-=`，`*=`，`+`，`-`，`*`运算符。


#### 6.与矩阵进行二元运算

1. 数据类型

   输入参数 `DynamicMatrix<Tp>` ，表示第二个操作矩阵。

   返回类型 `DynamicMatrix<Tp>` ，为结果矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   本方法要求两个矩阵的大小相同。

   包含 `+=`，`-=`，`*=`，`==`，`!=`，`+`，`-`运算符。

#### 7.与矩阵进行乘法运算

1. 数据类型

   输入参数 `const DynamicMatrix<Tp> &a` ，表示第一个矩阵。

   输入参数 `const DynamicMatrix<Tp> &b` ，表示第二个矩阵。

   返回类型 `DynamicMatrix<Tp>` ，表示两个矩阵的乘积。

2. 时间复杂度

   $O(m\times n\times l)$ 。

3. 备注

   本方法要求第一个矩阵的列数等于第二个矩阵的行数。

#### 8.快速幂(pow)

1. 数据类型

   输入参数 `uint64_t n` ，表示指数。

   返回类型 `DynamicMatrix<Tp>` ，表示结果矩阵。

2. 时间复杂度

   $O(m^3 \cdot \log n)$ 。

3. 备注

   本方法要求矩阵行数等于列数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/DynamicMatrix.h"
#include "MATH/StaticModInt32.h"

template <typename Matrix>
void print(Matrix &mat) {
    for (int i = 0; i < mat.row(); i++) {
        for (int j = 0; j < mat.column(); j++) {
            cout << mat[i][j] << (j == mat.column() - 1 ? '\n' : ' ');
        }
    }
    cout << "---------------------\n";
}
int main() {
    using elem = OY::mint1000000007;
    OY::DynamicMatrix<elem> A{{{1, 2, 3}, {4, 5, 6}}};
    A += 20;
    print(A);
    A -= 10;
    print(A);
    A *= 3;
    print(A);

    auto B = OY::DynamicMatrix<elem>::raw(3, 3, 10);
    print(B);

    auto C = OY::DynamicMatrix<elem>::unit(3);
    print(C);

    // 利用矩阵加速递推的例子：斐波那契数列
    // 假设 a[i] = x, a[i+1] = y, 则 a[i+2] = a[i] + a[i+1] = x + y
    // 由线性代数可知
    //
    //                     0   1
    //  {a[i], a[i+1]} × {       } = {a[i+1], a[i+2]}
    //                     1   1
    //
    // 不妨把右侧的矩阵叫做 P，则有
    //
    //                   0   1  ^n                   ^n
    //  {a[0], a[1]} × {       }   = {a[0], a[1]} × P   = {a[n], a[n+1]}
    //                   1   1
    //
    // 矩阵乘算虽然不满足交换律，但是符合结合律。我们可以用快速幂加速计算 P 的幂

    cout << "fibonacci[0] = 1\n";
    cout << "fibonacci[1] = 1\n";
    OY::DynamicMatrix<elem> init{{{1, 1}}};
    OY::DynamicMatrix<elem> P{{{0, 1}, {1, 1}}};
    // 计算 a[10]%1000000007
    cout << "fibonacci[10] = " << (init * P.pow(10))[0][0] << endl;
    // 计算 a[100000000000000]%1000000007
    cout << "fibonacci[100000000000000] = " << (init * P.pow(100000000000000))[0][0] << endl;
}
```

```
#输出如下
21 22 23
24 25 26
---------------------
11 12 13
14 15 16
---------------------
33 36 39
42 45 48
---------------------
10 10 10
10 10 10
10 10 10
---------------------
1 0 0
0 1 0
0 0 1
---------------------
fibonacci[0] = 1
fibonacci[1] = 1
fibonacci[10] = 89
fibonacci[100000000000000] = 481075011

```

