### 一、模板类别

​	数学：素性测试。

​	练习题目：

1. [#143. 质数判定](https://loj.ac/p/143)
2. [P5736 【深基7.例2】质数筛](https://www.luogu.com.cn/problem/P5736)
3. [Primality Test](https://judge.yosupo.jp/problem/primality_test)(https://github.com/yosupo06/library-checker-problems/issues/880)



### 二、模板功能

1. 数据类型

   模板参数 `typename Tp` ，可以为 `uint32_t` 或者 `uint64_t` ，表示元素类型。

   输入参数 `Tp n` ，表示要查询的数。

2. 时间复杂度

   $O(\log n)$ ，包含乘法运算，无除法运算。

3. 备注

   本模板用于对某一数字进行素性检验。
   
   如果要对大量的数字进行素性检验，更有效的方法是用筛法生成素数表，然后查表。
   
   **注意：** `32` 位版只支持对 `[0,2^30)` 范围内的数字的素性测试；`64` 位版只支持对 `[0,2^62)` 范围内的数字的素性测试。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/PrimeCheck.h"

int main() {
    for (int i = 1000; i <= 1100; i++) {
        if (OY::is_prime32(i)) {
            cout << i << " is prime\n";
        }
    }
}
```

```
#输出如下
1009 is prime
1013 is prime
1019 is prime
1021 is prime
1031 is prime
1033 is prime
1039 is prime
1049 is prime
1051 is prime
1061 is prime
1063 is prime
1069 is prime
1087 is prime
1091 is prime
1093 is prime
1097 is prime

```

