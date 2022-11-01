### 一、模板类别

​	数学：下降幂多项式。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `_Poly` ，表示多项式类。

2. 时间复杂度

3. 备注

   本模板仅包含一个成员变量，表示一个下降幂多项式每一项的系数。
   
   下文中用 `ffpoly` 表示本类。
   
    `ffpoly` 的构造方法，或者是在默认构造之后，手动填写各项系数；或者是从普通函数或者点值表示转化而来（参见后续说明）。
   
   设 `a[i]` 表示最高次为 `n` 次的下降幂多项式 `p` 的各项系数，则有：$$p= \sum_{i=0}^{n}a_i \cdot \prod_{j=0}^{i-1}(x-j)$$ 。


#### 2.预备阶乘

1. 数据类型

   输入参数 `uint32_t __length` ，表示要预备的阶乘范围。

2. 时间复杂度

   $O(n)$ 。
   
3. 备注

   在需要使用阶乘或者阶乘的逆的时候，本函数会自动被调用。

#### 3.获取 e^x

1. 数据类型

   输入参数 `uint32_t __length` ，表示要获取的多项式的长度。

   返回类型 `_Poly` ，表示得到的多项式。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数返回生成函数 $e^x$ 的多项式展开。

#### 4.获取 e^-x

1. 数据类型

   输入参数 `uint32_t __length` ，表示要获取的多项式的长度。

   返回类型 `_Poly` ，表示得到的多项式。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数返回生成函数 $e^{-x}$ 的多项式展开，或者说是生成函数 $e^x$ 的多项式展开的逆。

#### 5.从点值转化为下降幂

1. 数据类型

   输入参数 `_Poly __ys` ，表示 `[0, n-1]` 范围内每个自变量对应的因变量值。

   返回类型 `ffpoly` ，表示得到的下降幂多项式。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本函数从从 `0` 开始的连续区间上的点值，转化得到下降幂多项式。

#### 6.从下降幂转化为点值

1. 数据类型

   输入参数 `const ffpoly &__a` ，表示下降幂多项式。

   返回类型 `_Poly` ，表示下降幂多项式在从 `0` 开始的连续区间上的点值。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本函数从下降幂，获取从 `0` 开始的连续区间上的点值。

#### 7.从普通多项式转化为下降幂

1. 数据类型

   输入参数 `const _Poly &__a` ，表示普通多项式。

   返回类型 `ffpoly` ，表示得到的下降幂多项式。

2. 时间复杂度

   $O(n\cdot\log^2 n)$ 。

3. 备注

   本函数从普通多项式，转化得到下降幂多项式。

#### 8.从下降幂转化为普通多项式

1. 数据类型

   输入参数 `const ffpoly &__a` ，表示下降幂多项式。

   返回类型 `_Poly` ，表示得到的普通多项式。

2. 时间复杂度

   $O(n\cdot\log^2 n)$ 。

3. 备注

   本函数从下降幂，转化得到普通多项式。

#### 9.乘法运算符

1. 数据类型

2. 时间复杂度

   $O(n\cdot\log n)$ 。

#### 10.计算连续点值

1. 数据类型

   输入参数 `uint32_t __range` ，表示计算点值的范围。

   返回类型 `_Poly` ，表示得到的点值。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本函数计算 `[0, __range-1]` 范围内的点值。

#### 11.计算单点点值

1. 数据类型

   输入参数 `_Tp __x` ，表示自变量的值。

   返回类型 `_Tp` ，表示对应的因变量的值

2. 时间复杂度

   $O(n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/FallingFactorialPolynomial.h"

using mint = OY::StaticModInt32<998244353, true>;
using poly = OY::NTTPolynomial<mint, 1 << 18>;
using ffpoly = OY::FallingFactorialPolynomial<poly>;
int main() {
    // 只要用到 ntt 记得设置原根
    poly::s_primitiveRoot = 3;

    ffpoly P;
    P.m_coef.push_back(1);
    P.m_coef.push_back(2);
    P.m_coef.push_back(3);
    P.m_coef.push_back(1);
    cout << "P = " << P.m_coef[0] << " + " << P.m_coef[1] << "*x + " << P.m_coef[2] << "*x(x-1) + " << P.m_coef[3] << "*x(x-1)(x-2)" << endl;

    // 计算 [0, 3] 的点值
    cout << P.calcRange(4) << endl;
    // 单点计算，验算一下
    for (int i = 0; i < 4; i++) cout << P.calc(i) << (i == 3 ? '\n' : ' ');

    // 从点值获取下降幂
    auto P2 = ffpoly::fromPoints(P.calcRange(4));
    cout << "P2 = " << P2.m_coef[0] << " + " << P2.m_coef[1] << "*x + " << P2.m_coef[2] << "*x(x-1) + " << P2.m_coef[3] << "*x(x-1)(x-2)" << endl;

    // 从下降幂转化为普通多项式
    auto p = ffpoly::toNormal(P);
    cout << "p = " << p[0] << " + " << p[1] << "*x + " << p[2] << "*x^2 + " << p[3] << "*x^3" << endl;

    // 用普通多项式进行多点求值验算
    poly xs{0, 1, 2, 3};
    auto ys = p.calc(xs);
    cout << ys << endl;
}
```

```
#输出如下
P = 1 + 2*x + 3*x(x-1) + 1*x(x-1)(x-2)
1 3 11 31 
1 3 11 31
P2 = 1 + 2*x + 3*x(x-1) + 1*x(x-1)(x-2)
p = 1 + 1*x + 0*x^2 + 1*x^3
1 3 11 31 

```
