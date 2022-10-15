### 一、模板类别

​	数学：连续点平移插值。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `_Poly` ，表示多项式类。

   构造参数 `const _Poly& __ys` ，表示一些连续且递增的自变量，所对应的因变量值。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板根据给出的 `n` 个连续点，拟合一个不超过 `n-1` 次的多项式。当输入一个平移值时，本模板可以在 $O(n\cdot\log n)$ 的时间内给出一个长度为 `n` 的自变量窗口内的所有因变量值。但是，本模板并不能给出多项式每一项的系数值。


#### 2.计算平移后的窗口因变量值（无交界）

1. 数据类型

   输入参数 `_Tp __shift` ，表示窗口平移量。

   返回类型 `_Poly` ，表示经过平移后的自变量窗口，每个自变量对应的因变量。

2. 时间复杂度

   $O(n\cdot \log n)$ 。
   
3. 备注

   本函数对应的平移量为 `[n, P-n]` ，其中 `P` 表示模数，`n` 表示窗口长度。

   输入参数的类型为 `_Tp` ，是因为多项式具有和模数一样的周期。

#### 3.计算平移后的窗口因变量值

1. 输入参数 `_Tp __shift` ，表示窗口平移量。

   返回类型 `_Poly` ，表示经过平移后的自变量窗口，每个自变量对应的因变量。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   本函数对应的平移量为 `[0, P-1)` ，其中 `P` 表示模数。

   输入参数的类型为 `_Tp` ，是因为多项式具有和模数一样的周期。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/LagrangeInterpolation_ex.h"
#include "MATH/NTTPolynomial.h"
#include "MATH/StaticModInt.h"

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
    OY::LagrangeInterpolation_ex<poly> li({P.calc(2), P.calc(3), P.calc(4)});

    // 输出当 x∈[0, 2] 时，y 的值
    auto res = li.calc(-2);
    for (int x = 0; x <= 2; x++)
        cout << "when x = " << x << ", y = " << res[x] << endl;

    // 输出当 x∈[3, 5] 时，y 的值
    res = li.calc(1);
    for (int x = 3; x <= 5; x++)
        cout << "when x = " << x << ", y = " << res[x - 3] << endl;

    // 输出当 x∈[3000, 3002] 时，y 的值
    res = li.calc(2998);
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

