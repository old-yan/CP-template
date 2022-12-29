### 一、模板类别

​	数学：分拆数打表。

### 二、模板功能

#### 1.多项式求分拆数

1. 数据类型

   模板参数 `typename _Poly` ，表示多项式类。

   输入参数 `uint32_t n` ，表示打表范围。

   返回类型 `_Poly` ，表示从 `0` 到 `n` 的分拆数。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   分拆数是表示将无标号元素划分到无标号集合中的方案数。

    $a_n$ 表示将 $n$ 个相同元素划分到若干个非空集合中的方案数。

   特别的，第 $0$ 项分拆数等于 $1$ 。 

4. 生成函数

   待补。

#### 2.递推求分拆数

1. 数据类型

   模板参数 `typename _Tp` ，表示数的类型。

   输入参数 `uint32_t n` ，表示打表范围。

   返回类型 `std::vector<_Tp>` ，表示从 `0` 到 `n` 的分拆数。

2. 时间复杂度

    $O(n\cdot\sqrt n)$ 。

3. 备注

   在 `n < 1e4`  的情况下，本方法较多项式方法要快。
   
   在 `n > 1e5` 的情况下，本方法显著慢于多项式方法。

#### 3.递推求互异分拆数/奇分拆数

1. 数据类型

   模板参数 `typename _Tp` ，表示数的类型。

   输入参数 `uint32_t n` ，表示打表范围。

   返回类型 `std::vector<_Tp>` ，表示从 `0` 到 `n` 的互异分拆数/奇分拆数。

2. 时间复杂度

   $O(n\cdot\sqrt n)$ 。

3. 备注

   互异分拆数是表示将无标号元素划分到无标号集合中，且每个集合大小互不相同的方案数。

   奇分拆数是表示将无标号元素划分到奇数个无标号集合中的方案数。

   互异分拆数恰等于奇分拆数。

    $a_n$ 表示将 $n$ 个相同元素划分到若干个非空集合中，且为互异分拆/奇分拆的方案数。

   特别的，第 $0$ 项互异分拆数/奇分拆数等于 $1$ 。 

#### 4.获取分拆数打表

1. 数据类型

   模板参数 `typename _Tp` ，表示数的类型。

   输入参数 `uint32_t n` ，表示打表范围。

   返回类型 `std::vector<std::vector<_Tp>>` ，表示各种分拆的方案数。

2. 时间复杂度

    $O(n^2)$ 。

3. 备注

   返回的二维数组中，第 `i` 行第 `j` 列的数表示将 `i` 个相同元素划分到 `j` 个相同集合中的方案数。

#### 5.获取互异分拆数打表

1. 数据类型

   模板参数 `typename _Tp` ，表示数的类型。

   输入参数 `uint32_t n` ，表示打表范围。

   返回类型 `std::vector<std::vector<_Tp>>` ，表示各种互异分拆的方案数。

2. 时间复杂度

   $O(n^2)$ 。

3. 备注

   返回的二维数组中，第 `i` 行第 `j` 列的数表示将 `i` 个相同元素划分到 `j` 个相同集合中，且各个集合大小互不相同的方案数。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/PartitionNumber.h"
#include <assert.h>

using mint = OY::StaticModInt32<998244353, true>;
using poly = OY::NTTPolynomial<mint, 1 << 10>;
int main() {
    poly::s_primitiveRoot = 3;

    // 分拆数可以求将 n 个相同元素放入若干个相同集合的方案数
    auto pn = OY::getPartitionNumber_poly<poly>(20);
    cout << "put 1 ball into some bags: " << pn[1] << " way\n";
    cout << "put 2 balls into some bags: " << pn[2] << " ways\n";
    cout << "put 3 balls into some bags: " << pn[3] << " ways\n";
    cout << "put 4 balls into some bags: " << pn[4] << " ways\n";
    cout << "put 5 balls into some bags: " << pn[5] << " ways\n";
    cout << "put 6 balls into some bags: " << pn[6] << " ways\n";

    // 数据规模较小时，多项式不如根号递推速度快
    auto pn2 = OY::getPartitionNumber<mint>(20);
    for (int i = 0; i < pn2.size(); i++) assert(pn[i] == pn2[i]);

    // 二维递推可以求出放入指定数量集合的方案数
    auto pn_table = OY::getPartitionNumberTable<mint>(20);
    cout << "put 6 balls into 1 bag: " << pn_table[6][1] << " way\n";
    cout << "put 6 balls into 2 bags: " << pn_table[6][2] << " ways\n";
    cout << "put 6 balls into 3 bags: " << pn_table[6][3] << " ways\n";
    cout << "put 6 balls into 4 bags: " << pn_table[6][4] << " ways\n";
    cout << "put 6 balls into 5 bags: " << pn_table[6][5] << " ways\n";
    cout << "put 6 balls into 6 bags: " << pn_table[6][6] << " ways\n";

    // 互异分拆数可以求将 n 个相同元素放入若干个相同集合，且各个集合大小互不相同的方案数
    auto pdn = OY::getDistinctPartitionNumber<mint>(20);
    cout << "put 1 ball into some bags: " << pdn[1] << " way\n";
    cout << "put 2 balls into some bags: " << pdn[2] << " ways\n";
    cout << "put 3 balls into some bags: " << pdn[3] << " ways\n";
    cout << "put 4 balls into some bags: " << pdn[4] << " ways\n";
    cout << "put 5 balls into some bags: " << pdn[5] << " ways\n";
    cout << "put 20 balls into some bags: " << pdn[20] << " ways\n";

    // 二维递推可以求出放入指定数量集合的方案数
    auto pdn_table = OY::getDistinctPartitionNumberTable<mint>(20);
    cout << "put 20 balls into 1 bag: " << pdn_table[20][1] << " way\n";
    cout << "put 20 balls into 2 bags: " << pdn_table[20][2] << " ways\n";
    cout << "put 20 balls into 3 bags: " << pdn_table[20][3] << " ways\n";
    cout << "put 20 balls into 4 bags: " << pdn_table[20][4] << " ways\n";
    cout << "put 20 balls into 5 bags: " << pdn_table[20][5] << " ways\n";
    cout << "put 20 balls into 6 bags: " << pdn_table[20][6] << " ways\n";
}
```

```
#输出如下
put 1 ball into some bags: 1 way
put 2 balls into some bags: 2 ways
put 3 balls into some bags: 3 ways
put 4 balls into some bags: 5 ways
put 5 balls into some bags: 7 ways
put 6 balls into some bags: 11 ways
put 6 balls into 1 bag: 1 way
put 6 balls into 2 bags: 3 ways
put 6 balls into 3 bags: 3 ways
put 6 balls into 4 bags: 2 ways
put 6 balls into 5 bags: 1 ways
put 6 balls into 6 bags: 1 ways
put 1 ball into some bags: 1 way
put 2 balls into some bags: 1 ways
put 3 balls into some bags: 2 ways
put 4 balls into some bags: 2 ways
put 5 balls into some bags: 3 ways
put 20 balls into some bags: 64 ways
put 20 balls into 1 bag: 1 way
put 20 balls into 2 bags: 9 ways
put 20 balls into 3 bags: 24 ways
put 20 balls into 4 bags: 23 ways
put 20 balls into 5 bags: 7 ways
put 20 balls into 6 bags: 0 ways

```

