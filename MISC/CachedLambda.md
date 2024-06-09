### 一、模板类别

​	数据结构：记忆化递归匿名函数。

​	练习题目：

1. [233. 数字 1 的个数](https://leetcode.cn/problems/number-of-digit-one)
2. [312. 戳气球](https://leetcode.cn/problems/burst-balloons)


### 二、模板功能

​		本模板功能模仿 `python` 中的 `cache` 装饰器，用于为递归匿名函数添加记忆化功能。

​		由于匿名函数从 `C++14` 起才支持泛型，所以通过 `auto &self` 或者 `auto &&self` 将自己传给自己的语法从 `C++14` 起才可用。创建这样一个匿名函数之后，通过 `make_CacheSolver` 即可将匿名函数转化为记忆化函数。

​		模板参数 `MakeRecord` 用于记录每条记忆化结果，当 `MakeRecord` 为 `true` 时，可以支持 `clear_cache` 方法。在一些多测场景下，需要改变背景信息重新进行搜索的情况下，需要 `cache_clear` 方法。

​		模板参数 `size_type BUFFER` 表示为记忆化而开的哈希表的结点池大小。显然， `BUFFER` 应当大于状态数，以避免过多的哈希碰撞；但也不宜过大，否则效率反而降低。一般建议为总状态数的 `1.5~2` 倍即可。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticModInt32.h"
#include "MISC/CachedLambda.h"

using mint = OY::mint1000000007;
void test_factorial() {
    // 第一个参数类型必须为 auto && 或者 auto &
    auto fac = [](auto &&self, int n) -> mint {
        if (!n) return 1;
        return self(self, n - 1) * n;
    };

    // 需要指定两个参数
    // MakeRecord 表示是否记录每条结果，以便清空 cache
    // BUFFER 表示哈希表池子大小
    auto cached_fac = OY::make_CacheSolver<false, 1 << 16>(fac);

    cout << "fac[3] = " << cached_fac.query(3) << endl;
    cout << "fac[4] = " << cached_fac.query(4) << endl;
    cout << "fac[5] = " << cached_fac.query(5) << endl;

    // 输出 fac[10000~20000] 的和
    mint sum = 0;
    for (int i = 10000; i <= 20000; i++) sum += cached_fac.query(i);
    cout << "sum of fac[10000~20000] = " << sum << endl
         << endl;
}

void test_fibonacci() {
    auto cached_fib = OY::make_CacheSolver<false, 1 << 10>(
        [](auto &&self, int n) -> mint {
            return self(self, n - 1) + self(self, n - 2);
        });
    // 因为没有写 n<=1 时的初始值，所以需要手动设置递归出口
    cached_fib.set_initial_value(0, 1);
    cached_fib.set_initial_value(1, 1);

    cout << "fib[10] = " << cached_fib.query(10) << endl;
    cout << "fib[100] = " << cached_fib.query(100) << endl;
    cout << "fib[1000] = " << cached_fib.query(1000) << endl
         << endl;
}

void test_comb() {
    auto cached_comb = OY::make_CacheSolver<false, 1 << 20>(
        [](auto &self, int n, int m) -> mint {
            if (!m) return 1;
            if (!n) return 0;
            return self(self, n - 1, m - 1) + self(self, n - 1, m);
        });

    cout << "comb[1000, 500] = " << cached_comb.query(1000, 500) << endl
         << endl;
}

int main() {
    test_factorial();
    test_fibonacci();
    test_comb();
}
```

```
#输出如下
fac[3] = 6
fac[4] = 24
fac[5] = 120
sum of fac[10000~20000] = 864712836

fib[10] = 89
fib[100] = 782204094
fib[1000] = 107579939

comb[1000, 500] = 159835829


```

