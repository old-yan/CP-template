### 一、模板类别

​	数学：卢卡斯定理（动态模数打表）。

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename _Tp` ，表示任意一种自取模数类。

2. 时间复杂度

   $O(P)$ ，此处 `P` 指模数。
   
3. 备注

   本模板为卢卡斯定理的预处理的版本，在一个模数下求多个组合数的情况下，进行预处理比较划算，所以优先使用本模板。

#### 2.查询组合数

1. 数据类型

   输入参数 `uint32_t __n` ，表示从 `__n` 个数中选取。

   输入参数 `uint32_t __m` ，表示选取 `__m` 个数。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法限制 `__n` 和 `__m` 在 `[0, __P)` 范围内。

#### 3.查询组合数

1. 数据类型

   输入参数 `long long __n` ，表示从 `__n` 个数中选取。

   输入参数 `long long __m` ，表示选取 `__m` 个数。

2. 时间复杂度

   $O(\log_p^m)$ 。

3. 备注

   本方法中 `__n` 和 `__m` 的范围无限制。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/LucasTable.h"

using mint = OY::StaticModInt32<23, true>;
int main() {
    OY::LucasTable<mint> table;

    //求组合数 C(15,2) mod 23
    auto a = table.rawQuery(15, 2);
    cout << "Comb(15, 2) mod 23 ≡ " << a << endl;

    //求组合数 C(1515,222) mod 23
    auto b = table.query(1515, 222);
    cout << "Comb(1515, 222) mod 23 ≡ " << b << endl;
}
```

```
#输出如下
Comb(15, 2) mod 23 ≡ 13
Comb(1515, 222) mod 23 ≡ 20

```

