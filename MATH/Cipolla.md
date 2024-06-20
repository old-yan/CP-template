### 一、模板类别

​	数学：二次剩余算法。

​	练习题目：

1. [P5491 【模板】二次剩余](https://www.luogu.com.cn/problem/P5491)
2. [Sqrt Mod](https://judge.yosupo.jp/problem/sqrt_mod)(https://github.com/yosupo06/library-checker-problems/issues/27)


### 二、模板功能

​	本模板用于判定二次剩余，以及求解同余平方根。需要注意的是，本模板只解决模数为质数的情况。

​	模板参数 `Tp` 可以传递任何一种自取模数。

​	返回值为 `0` 表示二次剩余的根不存在；若非零，则一定返回较小的那个根。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/Cipolla.h"
#include "MATH/StaticModInt32.h"

int main() {
    using mint = OY::StaticModInt32<7, true>;
    auto x = OY::Cipolla<mint>(2);
    cout << x << " * " << x << " ≡ " << mint(x) * x << " mod " << mint::mod() << "\n";
    cout << mint::mod() - x << " * " << mint::mod() - x << " ≡ " << mint(mint::mod() - x) * (mint::mod() - x) << " mod " << mint::mod() << "\n";

    auto x2 = OY::Cipolla<mint>(3);
    if (x2) {
        cout << "3 is quadratic residue mod " << mint::mod() << "\n";
    } else {
        cout << "3 is not quadratic residue mod " << mint::mod() << "\n";
    }
}
```

```
#输出如下
3 * 3 ≡ 2 mod 7
4 * 4 ≡ 2 mod 7
3 is not quadratic residue mod 7

```

