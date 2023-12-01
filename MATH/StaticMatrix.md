### 一、模板类别

​	数学：静态矩阵（长宽固定）。

​	练习题目：

1. [P1306 斐波那契公约数](https://www.luogu.com.cn/problem/P1306)
2. [P1939 矩阵加速（数列）](https://www.luogu.com.cn/problem/P1939)
3. [P1962 斐波那契数列](https://www.luogu.com.cn/problem/P1962)

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename Tp` ，表示元素的类型。

   模板参数 `uint32_t M` ，表示行数。

   模板参数 `uint32_t N` ，表示列数。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   本模板没有提供构造函数。在无参情况下，所有元素为默认值 `0` ；如果想要传递初始参数，可以使用花括号（初始化列表）。

#### 2.同值矩阵(raw)

1. 数据类型

   输入参数 `Tp a` ，表示元素值。

   返回类型 `StaticMatrix<Tp, M, N>` ，为所有元素值均为 `a` 的矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

#### 3.单位矩阵(unit)

1. 数据类型

   返回类型 `StaticMatrix<Tp, M, N>` ，为主对角线元素值均为 `1` 的矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   本方法要求 `M == N` ，除对角线外其余值为 `0` 。

#### 4.与元素进行二元运算

1. 数据类型

   输入参数 `Tp a` ，表示第二个操作数。

   返回类型 `StaticMatrix<Tp, M, N>` ，为结果矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   包含 `+=`，`-=`，`*=`，`+`，`-`，`*`运算符。


#### 5.与矩阵进行二元运算

1. 数据类型

   输入参数 `StaticMatrix<Tp, M, N>` ，表示第二个操作矩阵。

   返回类型 `StaticMatrix<Tp, M, N>` ，为结果矩阵。

2. 时间复杂度

   $O(m\times n)$ 。

3. 备注

   包含 `+=`，`-=`，`*=`，`==`，`!=`，`+`，`-`运算符。

#### 6.与矩阵进行乘法运算

1. 数据类型

   输入参数 `const StaticMatrix<Tp, M, N> &a` ，表示第一个矩阵。

   输入参数 `const StaticMatrix<Tp, N, L> &b` ，表示第二个矩阵。

   返回类型 `StaticMatrix<Tp, M, L>` ，表示两个矩阵的乘积。

2. 时间复杂度

   $O(m\times n\times l)$ 。

3. 备注

   本方法要求第一个矩阵的列数等于第二个矩阵的行数。

#### 7.快速幂(pow)

1. 数据类型

   输入参数 `uint64_t n` ，表示指数。

   返回类型 `StaticMatrix<Tp, M, N>` ，表示结果矩阵。

2. 时间复杂度

   $O(m^3 \cdot \log n)$ 。

3. 备注

   本方法要求 `M == N`  。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticMatrix.h"
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
    OY::StaticMatrix<elem, 2, 3> A{{{1, 2, 3}, {4, 5, 6}}};
    A += 20;
    print(A);
    A -= 10;
    print(A);
    A *= 3;
    print(A);

    OY::StaticMatrix<elem, 3, 3> B = OY::StaticMatrix<elem, 3, 3>::raw(10);
    print(B);

    OY::StaticMatrix<elem, 3, 3> C = OY::StaticMatrix<elem, 3, 3>::unit();
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
    OY::StaticMatrix<elem, 1, 2> init{{{1, 1}}};
    OY::StaticMatrix<elem, 2, 2> P{{{0, 1}, {1, 1}}};
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

