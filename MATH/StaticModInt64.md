### 一、模板类别

​	数学：自取模整数（静态模数）

​	练习题目：

1. [96. 不同的二叉搜索树](https://leetcode.cn/problems/unique-binary-search-trees/)
2. [P1044 [NOIP2003 普及组] 栈](https://www.luogu.com.cn/problem/P1044)
3. [P1595 信封问题](https://www.luogu.com.cn/problem/P1595)
4. [P3014 [USACO11FEB] Cow Line S](https://www.luogu.com.cn/problem/P3014)
5. [P8764 [蓝桥杯 2021 国 BC] 二进制问题](https://www.luogu.com.cn/problem/P8764)


### 二、模板功能

​	本模板为自取模数。

​	模数 `P` 为模板参数，且需要在编译期指定， `P` 的范围限定为 `[2, 2 ^ 62)`。同时可以指定 `IsPrime` 的逻辑值，当 `IsPrime` 为真时，可以通过费马小定理加速求逆元。

​	从无符号数构造比从有符号数构造的速度更快。如果在构造时，确定构造参数的范围在 `[0, P)` 内，可以通过静态方法 `raw` 进行更快的构造。

​	当 `IsPrime` 为真时，使用费马小定理算法计算逆元；否则使用扩展欧几里得算法计算逆元。

​	本类型的每个对象占用内存为 `8` 个字节。

​	本类型可以由各种数字类型隐式转化而来；但是必须显式地转换为 `int` ，`unsigned int` ，`long long` ，`unsigned long long` ，`double` ，`long double` 等类型。

​	本类型支持输入输出流操作，但是流式输入必须保证输入的数字已经处于 `[0, P)` 的范围内。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticModInt64.h"

#include <random>

void test_Fermat_10000019() {
    // 验证费马定理
    using mint = OY::StaticModInt64<10000019, true>;
    mint prod(1);
    for (auto i = 1; i != mint::mod() - 1; i++) {
        prod *= i;
    }
    cout << mint::mod() - 2 << "! mod " << mint::mod() << " = " << prod << endl;
}

void test_small_modint() {
    // 验证区间内随机数的和
    // 模数取个小点的偶数
    using mint = OY::StaticModInt64<16, false>;
    std::mt19937_64 rand;
    long long sum_ll = 0;
    mint sum_mint;
    uint64_t start = rand() % (1ull << 40), len = 10000001;
    for (uint64_t a = 0; a < len; a++) {
        sum_ll += start + a;
        sum_mint += start + a;
    }
    cout << "sum from " << start << " to " << start + len - 1 << " = " << sum_ll << endl;
    cout << "sum from " << start << " to " << start + len - 1 << " mod " << mint::mod() << " = " << sum_mint << endl;
}

void test_big_modint() {
    // 计算区间内随机数的乘积
    // 模数取个大点的质数
    // 验证就算了，可以用 python 验证
    using mint = OY::mint4611686018427387847;
    std::mt19937_64 rand;
    mint prod = 1;
    uint64_t start = rand(), len = 10000001;
    for (uint64_t a = 0; a < len; a++)
        prod *= start + a;
    cout << "prod from " << start << " to " << start + len - 1 << " mod " << mint::mod() << " = " << prod << endl;
}

int main() {
    test_Fermat_10000019();
    test_small_modint();
    test_big_modint();
}
```

```
#输出如下
10000017! mod 10000019 = 1
sum from 124402445990 to 124412445990 = 1244074584307445990
sum from 124402445990 to 124412445990 mod 16 = 6
prod from 14514284786278117030 to 14514284786288117030 mod 4611686018427387847 = 986469850098377379

```

