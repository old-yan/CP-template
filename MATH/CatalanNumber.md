### 一、模板类别

​	数学：卡特兰数打表。

​	练习题目：

1. [96. 不同的二叉搜索树](https://leetcode.cn/problems/unique-binary-search-trees/)
2. [P1044 [NOIP2003 普及组] 栈](https://www.luogu.com.cn/problem/P1044)

### 二、模板功能

#### 1.打表

1. 数据类型

   模板参数 `typename Tp` ，可以为任意的自取模数类。

   模板参数 `uint32_t MAX_RANGE` ，表示最大打表范围。

   输入参数 `uint32_t range` ，表示实际打表范围。

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

4. 生成函数

   待补。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/CatalanNumber.h"
#include "MATH/StaticModInt64.h"

using mint = OY::StaticModInt64<4611686018427387847, true>;
int main() {
    // 卡特兰数可以求合法括号序列及所有可以转换为合法括号序列模型的问题
    auto catalan_table = OY::CatalanNumber<mint, 30>(30);
    cout << "valid number of 3 pairs of brackets: " << catalan_table[3] << endl;
    cout << "valid number of 10 pairs of brackets: " << catalan_table[10] << endl;
    cout << "valid number of 20 pairs of brackets: " << catalan_table[20] << endl;
    cout << "valid number of 30 pairs of brackets: " << catalan_table[30] << endl;
}
```

```
#输出如下
valid number of 3 pairs of brackets: 5
valid number of 10 pairs of brackets: 16796
valid number of 20 pairs of brackets: 6564120420
valid number of 30 pairs of brackets: 3814986502092304

```

