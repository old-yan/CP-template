### 一、模板类别

​	数学：乘法逆元打表。

### 二、模板功能

#### 1.打表

1. 数据类型

   模板参数 `typename _Tp` ，可以使用任意一种自取模数类。

   构造参数 `long long __n` ，表示打表范围为 `[1,__n]` 。
   
2. 时间复杂度

   $O(n)$ 。

3. 备注

   本模板用于乘法逆元的线性打表。
   
   本模板不对参数进行检查，所以请保证 `__n` 大于 `1` ，且小于模数。
   
   **注意：** 本算法成立的前提是模数 `__P` 为质数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/InverseTable.h"

using mint = OY::StaticModInt32<1000000007, true>;
int main() {
    std::vector<mint> inv_table = OY::getInverseTable<mint>(10);
    for (int i = 1; i <= 10; i++) {
        auto inv = inv_table[i];
        cout << i << " * " << inv << " = " << inv * i << endl;
    }
}
```

```
#输出如下
1 * 1 = 1
2 * 500000004 = 1
3 * 333333336 = 1
4 * 250000002 = 1
5 * 400000003 = 1
6 * 166666668 = 1
7 * 142857144 = 1
8 * 125000001 = 1
9 * 111111112 = 1
10 * 700000005 = 1

```

