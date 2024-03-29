### 一、模板类别

​	数学：自然溢出整数


### 二、模板功能

​	本模板为自然溢出的整数类的封装。

​	模数 `P` 为 `pow(2, W)` ，其中 `W` 为当前无符号整数类的比特数。显然，模数只可能是 `pow(2, 32)` 或者 `pow(2, 64)` 。模数并不能以当前无符号整数类的形式得到表示，因为 `uint32_t` 的最大值为 `pow(2, 32) - 1` ， `uint64_t` 的最大值为 `pow(2, 64) - 1` 。

​	显然，模数不是质数；但是对于任何奇数，都可以使用扩展欧几里得算法计算出相应的逆元。

​	本类型可以由各种数字类型隐式转化而来；但是必须显式地转换为 `int` ，`unsigned int` ，`long long` ，`unsigned long long` ，`double` ，`long double` 等类型。

​	本类型支持输入输出流操作。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"

#include <random>

void test() {
    // 计算区间内随机数的和
    using mint = OY::mintu32;
    std::mt19937_64 rand;
    mint sum = {};
    uint64_t start = rand(), len = 10000001;
    for (uint64_t a = 0; a < len; a++)
        sum += start + a;
    cout << "sum from " << start << " to " << start + len - 1 << " mod 4294967296 = " << sum << endl;
}

int main() {
    test();
}
```

```
#输出如下
sum from 14514284786278117030 to 14514284786288117030 mod 4294967296 = 935112934

```

