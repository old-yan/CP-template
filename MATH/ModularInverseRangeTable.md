### 一、模板类别

​	数学：乘法逆元打表。

​	练习题目：

1. [P3811 【模板】模意义下的乘法逆元](https://www.luogu.com.cn/problem/P3811)

### 二、模板功能

#### 1.打表

1. 数据类型

   模板参数 `typename Tp` ，可以使用任意一种自取模数类。
   
   模板参数 `uint32_t MAX_RANGE` ，表示逆元打表的范围。

   构造参数 `uint32_t range` ，表示打表范围为 `[1, range]` 。默认为 `MAX_RANGE` 。
   
2. 时间复杂度

   $O(n)$ ，此处 `n` 为 `range` 的大小。

3. 备注

   本模板用于乘法逆元的线性打表。
   
   本模板不对参数进行检查，所以请保证 `n` 大于 `1` ，且小于模数。
   
   **注意：** 本算法成立的前提是模数为质数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/ModularInverseRangeTable.h"
#include "MATH/StaticModInt32.h"

int main() {
    using mint = OY::mint1000000007;
    OY::ModularInverseRangeTable<mint, 1000> table(100);
    for (int i = 1; i <= 10; i++) {
        auto inv = table[i];
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

