### 一、模板类别

​	数学：乘法逆元。

​	练习题目：

1. [P5431 【模板】模意义下的乘法逆元 2](https://www.luogu.com.cn/problem/P5431)

### 二、模板功能

​	本模板用于对给定区间内的若干个数求逆元，返回一个存放每个数的逆元结果的 `std::vector` 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/ModularInverseTable.h"
#include "MATH/StaticModInt32.h"

int main() {
    using mint = OY::mint1000000007;

    int A[3] = {1000, 2000, 3000};
    auto inverse_table = OY::get_modular_inverse_table<mint>(A, A + 3);
    for (int i = 0; i < 3; i++) {
        mint a = A[i];
        mint inv = inverse_table[i];
        cout << a << " * " << inv << " = " << a * inv << endl;
    }
}
```

```
#输出如下
1000 * 857000006 = 1
2000 * 428500003 = 1
3000 * 952333340 = 1

```

