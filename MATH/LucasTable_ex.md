### 一、模板类别

​	数学：扩展卢卡斯定理。

​	练习题目：

1. [P4720 【模板】扩展卢卡斯定理/exLucas](https://www.luogu.com.cn/problem/P4720)

### 二、模板功能

#### 1.构造

1. 数据类型

   模板参数 `typename Tp` ，表示任意一种自取模数类。
   
   模板参数 `uint32_t MAX_PRIME` ，表示模数的最多的不同的质因子数量。默认为 `10` 。

2. 时间复杂度

    $O(P)$ ，此处 `P` 指模数。
   
3. 备注

   在一个模数下求多个组合数的情况下，可以先针对模数进行预处理，以便进行高效查询。
   
   本模板的模数不限定为质数。

#### 2.查询组合数(query)

1. 数据类型

   输入参数 `uint64_t n` ，表示从 `n` 个数中选取。

   输入参数 `uint64_t m` ，表示选取 `m` 个数。

2. 时间复杂度

   $O(k\cdot \log m)$ ，其中 `k` 为模数的不同质因子数量。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/DynamicModInt32.h"
#include "MATH/LucasTable_ex.h"

int main() {
    using mint = OY::DynamicModInt32<0>;
    // 设置模数为 7265082
    mint::set_mod(2 * 3 * 11 * 11 * 10007, false);
    cout << mint::mod() << endl;

    OY::LucasTable_ex<mint> table;

    // 求组合数 C(15, 2) mod 7265082
    auto a = table.query(15, 2);
    cout << "Comb(15, 2) mod " << mint::mod() << " ≡ " << a << endl;

    // 求组合数 C(1515, 222) mod 7265082
    auto b = table.query(1515, 222);
    cout << "Comb(1515, 222) mod " << mint::mod() << " ≡ " << b << endl;
    
    // 求组合数 C(1515151515151515, 222222222222) mod 7265082
    auto c = table.query(1515151515151515, 222222222222);
    cout << "Comb(1515151515151515, 222222222222) mod " << mint::mod() << " ≡ " << c << endl;
}
```

```
#输出如下
7265082
Comb(15, 2) mod 7265082 ≡ 105
Comb(1515, 222) mod 7265082 ≡ 113256
Comb(1515151515151515, 222222222222) mod 7265082 ≡ 4757478

```

