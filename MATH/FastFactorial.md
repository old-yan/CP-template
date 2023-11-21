### 一、模板类别

​	数学：快速阶乘。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `_Poly` ，表示多项式类。

   构造参数 `_Tp __limit` ，表示最多要计算几的阶乘。默认为模数减一。

2. 时间复杂度

   $O(n\cdot\log n)$ ，此处 `n` 指 `__limit` 的值。

3. 备注

   本模板对特定模数进行预处理，然后可以较快地查询阶乘值。


#### 2.查询阶乘值

1. 数据类型

   输入参数 `_Tp __n` ，表示要查询 `__n` 的阶乘。

   返回类型 `_Tp` ，表示阶乘值。

2. 时间复杂度

   $O(\sqrt n)$ 。
   

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/FastFactorial.h"
#include "MATH/NTTPolynomial.h"
#include "MATH/StaticModInt32.h"

using mint = OY::StaticModInt32<998244353, true>;
using poly = OY::NTTPolynomial<mint, 1 << 18>;
int main() {
    // 只要用到 ntt 记得设置原根
    poly::s_primitiveRoot = 3;

    //预处理
    OY::FastFactorial<poly> facts(100000000);
    //查询阶乘
    cout << "98765432! = " << facts.query(98765432) << endl;
}
```

```
#输出如下
98765432! = 285611063

```

