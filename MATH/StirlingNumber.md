### 一、模板类别

​	数学：第二类斯特林数。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename _Poly` ，表示多项式类。

   构造参数 `uint32_t __n` ，表示要计算的最大范围。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   由于在计算中大量用到乘法逆元，所以在构造函数中对乘法逆元进行打表。
   
   又由于在计算幂函数时需要用到质数筛优化，所以在构造函数中进行埃氏筛预处理。
   
   本模板提供第二类斯特林数完整打表和按行打表的功能。
   
   模板参数 `_Poly` 不仅仅限制了多项式类，而且间接地通过多项式类限制了自取模数类，也就是限制了模数。


#### 2.完整打表

1. 数据类型

   输入参数 `uint32_t __n` ，表示打表范围。

   返回类型 `std::vector<_Poly>` ，表示打表结果。

2. 时间复杂度

   $O(n^2)$ 。
   
3. 备注

   本方法返回长度为 `__n+1` 的二维数组，表示从 `0` 到 `__n` 行的第二类斯特林数，其中 `i` 行的长度是 `i+1` 。

#### 3.按行打表

1. 数据类型

   输入参数 `uint32_t __n` ，表示打表行号。

   返回类型 `_Poly` ，表示查询当行的第二类斯特林数。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/FastFactorial.h"
#include "MATH/NTTPolynomial.h"
#include "MATH/StaticModInt.h"

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

