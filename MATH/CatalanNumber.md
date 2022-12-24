### 一、模板类别

​	数学：卡特兰数打表。

### 二、模板功能

#### 1.打表

1. 数据类型

   模板参数 `typename _Tp` ，可以为任意的自取模数类。

   输入参数 `uint32_t n` ，表示打表范围。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   由于在计算中大量用到乘法逆元，所以在构造函数中对乘法逆元进行打表。

   如果遇到以下递推式，表明数列为卡特兰数：
   
   1. $a_i = \frac{\binom{2 i}{i}}{i+1}$ ；
   2. $a_i = \begin{cases} \sum_{k=1}^{n} a_{k-1} a_{i-k} & i \geq 2, i \in \mathbf{N_{+}}\\ 1 & i = 0, 1 \end{cases} $ ；
   3. $a_i=\frac{a_{i-1}\cdot(4i-2)}{i+1}$ ；
   4. $a_i=\binom{2i}{i}-\binom{2i}{i-1}$ 。
   
   合法括号序列问题满足第二个递推式，所以结果为卡特兰数；
   
   可能的二叉搜索树方案数满足第二个递推式，所以结果也为卡特兰数。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/CatalanNumber.h"

using mint = OY::StaticModInt32<1000000007, true>;
int main() {
    std::vector<mint> catalan_table = OY::CatalanNumber<mint>(20);
    //卡特兰数可以求合法括号序列及所有可以转换为合法括号序列模型的问题
    cout << "valid number of 3 pairs of brackets: " << catalan_table[3] << endl;
    cout << "valid number of 10 pairs of brackets: " << catalan_table[10] << endl;
}
```

```
#输出如下
valid number of 3 pairs of brackets: 5
valid number of 10 pairs of brackets: 16796

```

