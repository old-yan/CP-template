### 一、模板类别

​	数学：贝尔数打表。

### 二、模板功能

#### 1.打表

1. 数据类型

   模板参数 `typename _Poly` ，表示多项式类。

   输入参数 `uint32_t n` ，表示打表范围。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   贝尔数是表示有标号元素划分到无标号集合中的方案数。

    $a_n$ 表示将 $n$ 个不同元素划分到若干个非空集合中的方案数。

   特别的，第 $0$ 项贝尔数等于 $1$ 。
   
   如果遇到以下递推式，表明数列为贝尔数：$a_n = \sum_{i=0}^{n-1}{\binom{n-1}{i}} a_i$ 。
   
4. 生成函数

   待补。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/BellNumber.h"

using mint = OY::StaticModInt32<998244353, true>;
using poly = OY::NTTPolynomial<mint, 1 << 10>;
int main() {
    poly::s_primitiveRoot = 3;

    // 贝尔数可以求将 n 个不同元素放入若干个相同集合的方法数
    auto bell_table = OY::BellNumber<poly>(20);
    cout << "put 1 ball into some bags: " << bell_table[1] << " way\n";
    cout << "put 2 balls into some bags: " << bell_table[2] << " ways\n";
    cout << "put 3 balls into some bags: " << bell_table[3] << " ways\n";
    cout << "put 4 balls into some bags: " << bell_table[4] << " ways\n";
    cout << "put 5 balls into some bags: " << bell_table[5] << " ways\n";
    cout << "put 6 balls into some bags: " << bell_table[6] << " ways\n";
}
```

```
#输出如下
put 1 ball into some bags: 1 way
put 2 balls into some bags: 2 ways
put 3 balls into some bags: 5 ways
put 4 balls into some bags: 15 ways
put 5 balls into some bags: 52 ways
put 6 balls into some bags: 203 ways

```

