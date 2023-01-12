### 一、模板类别

​	数学：卢卡斯定理，扩展卢卡斯定理。

### 二、模板功能

   本模板分为普通版和扩展版，普通版用于解决模数为质数的情况，而扩展版可以进一步解决模数为合数的情况。

#### 1.构造

1. 数据类型

   模板参数 `typename _Tp` ，表示任意一种自取模数类。

2. 时间复杂度

    $O(P)$ ，此处 `P` 指模数。
   
3. 备注

   在一个模数下求多个组合数的情况下，可以先针对模数进行预处理，以便进行高效查询。

#### 2.查询组合数

1. 数据类型

   输入参数 `uint32_t __n` ，表示从 `__n` 个数中选取。

   输入参数 `uint32_t __m` ，表示选取 `__m` 个数。

2. 时间复杂度

   在普通版中，为 $O(1)$ 。

   在扩展版中，为 $O(k\cdot \log P)$ ，其中 `k` 为模数的不同质因子数量。对于 `64` 位以内的整数， `k` 不会超过 `15` 。

3. 备注

   本方法限制 `__n` 和 `__m` 在 `[0, __P)` 范围内。

#### 3.查询组合数

1. 数据类型

   输入参数 ``uint64_t __n` ，表示从 `__n` 个数中选取。

   输入参数 ``uint64_t __m` ，表示选取 `__m` 个数。

2. 时间复杂度

   在普通版中，为 $O(\log_p^m)$ 。

   在扩展版中，为 $O(k\cdot \log m)$ ，其中 `k` 为模数的不同质因子数量。对于 `64` 位以内的整数， `k` 不会超过 `15` 。

3. 备注

   本方法中 `__n` 和 `__m` 的范围无限制。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/LucasTable.h"

using mint = OY::StaticModInt32<23, true>;
using mint2 = OY::DynamicModInt32<120>;
int main() {
    OY::LucasTable<mint> table;

    // 求组合数 C(15,2) mod 23
    auto a = table.rawQuery(15, 2);
    cout << "Comb(15, 2) mod 23 ≡ " << a << endl;

    // 求组合数 C(1515,222) mod 23
    auto b = table.query(1515, 222);
    cout << "Comb(1515, 222) mod 23 ≡ " << b << endl;

    OY::LucasTable_ex<mint2> table2;

    // 求组合数 C(15,4) mod 120
    auto a2 = table2.rawQuery(15, 4);
    cout << "Comb(15, 4) mod 120 ≡ " << a2 << endl;

    // 求组合数 C(1515,222) mod 120
    auto b2 = table2.query(1515, 222);
    cout << "Comb(1515, 222) mod 120 ≡ " << b2 << endl;
}
```

```
#输出如下
Comb(15, 2) mod 23 ≡ 13
Comb(1515, 222) mod 23 ≡ 20
Comb(15, 4) mod 120 ≡ 30
Comb(1515, 222) mod 120 ≡ 0

```

