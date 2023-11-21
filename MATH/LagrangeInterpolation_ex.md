### 一、模板类别

​	数学：连续点平移插值。

### 二、模板功能

​	本模板根据给出的 `n` 个连续点，拟合一个不超过 `n-1` 次的多项式。当输入一个平移值时，本模板可以在 $O(n\cdot\log n)$ 的时间内给出一个长度为 `n` 的自变量窗口内的所有因变量值。但是，本模板并不能给出多项式每一项的系数值。

#### 1.构造

1. 数据类型

   模板参数 `_Poly` ，表示多项式类。

   构造参数 `uint32_t __length` ，表示平移区间的长度。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   参数 `__length` 指定了平移区间长度，不同平移区间长度的预处理是不一样的。


#### 2.对给定连续区间点值进行预处理

1. 数据类型

   输入参数 `const _Poly&__ys` ，表示长度为 `__length` 的连续区间上的点值。

   返回类型 `_Solver` ，表示一个可以解答平移后的点值的解答器。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本方法返回的是一个解答器，通过调用解答器的解答方法，可以获取平移后的点值。

   调用 `rawCalc` 方法，时间复杂度 $O(n\cdot\log n)$ ，对应的平移量为 `[n, P-n]` ，其中 `P` 表示模数，`n` 表示窗口长度。

   调用 `calc` 方法，时间复杂度 $O(n\cdot\log n)$ ，对应的平移量为 `[0, P-1]` ，其中 `P` 表示模数，`n` 表示窗口长度。

   输入参数的类型为 `_Tp` ，是因为多项式具有和模数一样的周期。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/LagrangeInterpolation_ex.h"
#include "MATH/NTTPolynomial.h"
#include "MATH/StaticModInt32.h"

using mint = OY::StaticModInt32<998244353, true>;
using poly = OY::NTTPolynomial<mint, 1 << 12>;
int main() {
    // !!! 一定记得设置原根
    poly::s_primitiveRoot = 3;

    // 假定某个多项式的表达式为 y = 2 + x + 3 x^2
    poly P{2, 1, 3};
    // 计算当 x∈[2, 4] 时，y 的值
    for (int x = 2; x <= 4; x++)
        cout << "when x = " << x << ", y = " << P.calc(x) << endl;

    // 基于这三个点，我们建立拉格朗日插值
    OY::LagrangeInterpolation_ex<poly> li(3);
    auto solver = li.prepare({P.calc(2), P.calc(3), P.calc(4)});

    // 输出当 x∈[0, 2] 时，y 的值
    auto res = solver.calc(-2);
    for (int x = 0; x <= 2; x++)
        cout << "when x = " << x << ", y = " << res[x] << endl;

    // 输出当 x∈[3, 5] 时，y 的值
    res = solver.calc(1);
    for (int x = 3; x <= 5; x++)
        cout << "when x = " << x << ", y = " << res[x - 3] << endl;

    // 输出当 x∈[3000, 3002] 时，y 的值
    res = solver.calc(2998);
    for (int x = 3000; x <= 3002; x++)
        cout << "when x = " << x << ", y = " << res[x - 3000] << endl;
}
```

```
#输出如下
when x = 2, y = 16
when x = 3, y = 32
when x = 4, y = 54
when x = 0, y = 2
when x = 1, y = 6
when x = 2, y = 16
when x = 3, y = 32
when x = 4, y = 54
when x = 5, y = 82
when x = 3000, y = 27003002
when x = 3001, y = 27021006
when x = 3002, y = 27039016

```

