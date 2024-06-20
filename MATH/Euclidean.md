### 一、模板类别

​	数学：类欧几里得算法。

​	练习题目：

1. [P5170 【模板】类欧几里得算法](https://www.luogu.com.cn/problem/P5170)
2. [Sum of Floor of Linear](https://judge.yosupo.jp/problem/sum_of_floor_of_linear)(https://github.com/yosupo06/library-checker-problems/issues/48)


### 二、模板功能

   本模板虽然名为欧几里得，但是实际上和欧几里得根本没有关系。

#### 1.计算(calc)

1. 数据类型

   输入参数 `uint32_t a` ，表示参数。

   输入参数 `uint32_t b` ，表示参数。

   输入参数 `uint32_t c` ，表示参数。

   输入参数 `uint32_t n` ，表示参数。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指 `a` 和 `c` 中的最大值。

3. 备注

   本方法计算三个数列的和，一般不会单独计算其中的某一个和。
   
   使用 `auto [f, g, h] = obj.calc(a, b, c, n);`获取三个答案，`f, g, h` 意义如下：
   
   $f=\sum_{i=0}^n \lfloor \frac{a\times i+b}{c}\rfloor$ ；
   
   $g=\sum_{i=0}^n \lfloor \frac{a\times i+b}{c}\rfloor\times i$ ；
   
   $h=\sum_{i=0}^n \lfloor \frac{a\times i+b}{c}\rfloor^2$ 。

#### 2.计算整除和(Euclidean_floor_sum)

1. 数据类型

   同上。

2. 时间复杂度

   同上。

3. 备注

   本方法单独计算一个数列的和：
   
   $\sum_{i=0}^n \lfloor \frac{a\times i+b}{c}\rfloor$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/Euclidean.h"
#include "MATH/StaticModInt32.h"

int main() {
    uint32_t a = 12;
    uint32_t b = 7;
    uint32_t c = 25;
    uint32_t n = 10000;
    using mint = OY::mint1000000007;

    uint64_t F = 0, G = 0, H = 0;
    for (int i = 0; i <= n; i++) {
        F += (uint64_t(a) * i + b) / c;
        F %= mint::mod();
        G += (uint64_t(a) * i + b) / c * i;
        G %= mint::mod();
        H += (uint64_t(a) * i + b) / c * ((uint64_t(a) * i + b) / c);
        H %= mint::mod();
    }

    auto res = OY::Euclidean<mint>().calc(a, b, c, n);
    auto f = res.m_f, g = res.m_g, h = res.m_h;
    cout << F << "," << f << endl;
    cout << G << "," << g << endl;
    cout << H << "," << h << endl;
}
```



```
#输出如下
24000400,24000400
13995080,13995080
801916668,801916668

```

