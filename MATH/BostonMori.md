### 一、模板类别

​	数学： `Boston-Mori` 算法。

### 二、模板功能

#### 1.分式求某项系数

1. 数据类型

   模板参数 `typename _Poly` ，表示多项式类。

   模板参数 `typename _Tp` ，表示多项式类所确定的元素类型。

   输入参数 `const _Poly& a` ，表示分子多项式。

   输入参数 `const _Poly& b` ，表示分母多项式。

   输入参数 `uint64_t n` ，表示要求系数的项的次数。

2. 时间复杂度

   $O(m\cdot \log m\cdot \log n)$ ，此处 `m` 表示分子、分母多项式长度之和， `n` 表示要求系数的项的次数。

3. 备注

   使用 `a * b.inv()` 可以得到多项式的每项系数。然而，当 `n` 较大时，不能使用这个方法；有时也并不需要求所有项的系数。

   本方法可以在对数轮次内计算得到某一项的系数。

   **注意：** 本方法要求分母多项式 `b` 不为空，且常数项不为零。

#### 2.常系数齐次线性递推

1. 数据类型

   模板参数 `typename _Poly` ，表示多项式类。

   模板参数 `typename _Tp` ，表示多项式类所确定的元素类型。

   输入参数 `const _Poly& init` ，表示数列的初始元素。

   输入参数 `const _Poly& weight` ，表示递推公式的各个权重。要求和初始元素长度相等。

   输入参数 `uint64_t n` ，表示要求值的项的下标。

2. 时间复杂度

   $O(m\cdot \log m\cdot \log n)$ ，此处 `m` 表示递推长度， `n` 表示要求值的项的下标。

3. 备注

   常系数齐次线性递推，和多项式分式求某项系数，本质上为同一问题。

   本方法可以在对数轮次内计算得到某一项的值。

   **注意：** 本方法要求传入的 `init` 和 `weight` 长度相等。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/BostonMori.h"

using mint = OY::StaticModInt32<998244353, true>;
using poly = OY::NTTPolynomial<mint, 1 << 10>;
int main() {
    poly::s_primitiveRoot = 3;

    // 求 1 / (1 + 2 * x) 的 x ^ 4 的系数
    poly p{1};
    poly q{1, 2};
    cout << "coef of x^4: " << OY::BostonMori_div(p, q, 4) << endl;

    // 用朴素方法验证
    auto r = p.sizeTo(5) * q.sizeTo(5).inv();
    cout << r << endl;

    // 求斐波那契数列，但是用的是 ai = a_i-1 + a_i-3 + a_i-4
    poly init{0, 1, 1, 2};
    // 填写加权时， a_i-1 的权重排最前面
    // a_i-2 没有出现过，所以权重为零
    poly weight{1, 0, 1, 1};
    cout << "a4: " << OY::BostonMori_fill(init, weight, 4) << endl;
    cout << "a5: " << OY::BostonMori_fill(init, weight, 5) << endl;
    cout << "a6: " << OY::BostonMori_fill(init, weight, 6) << endl;
    cout << "a7: " << OY::BostonMori_fill(init, weight, 7) << endl;
    cout << "a8: " << OY::BostonMori_fill(init, weight, 8) << endl;
    cout << "a9: " << OY::BostonMori_fill(init, weight, 9) << endl;
}
```

```
#输出如下
coef of x^4: 16
1 998244351 4 998244345 16 
a4: 3
a5: 5
a6: 8
a7: 13
a8: 21
a9: 34

```

