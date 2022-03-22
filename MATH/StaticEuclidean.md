### 一、模板类别

​	数学：类欧几里得算法（静态模数）。

### 二、模板功能

   本模板虽然名为欧几里得，但是实际上和欧几里得根本没有关系。

#### 1.计算

1. 数据类型

   输入参数 `uint32_t __a` ，表示参数。

   输入参数 `uint32_t __b` ，表示参数。

   输入参数 `uint32_t __c` ，表示参数。

   输入参数 `uint32_t __n` ，表示参数。

2. 时间复杂度

   $O(\log n)$ ，此处 `n` 指 `__a` 和 `__c` 中的最大值。

3. 备注

   本方法计算三个数列的和，一般不会单独计算其中的某一个和。
   
   使用 `auto [f,g,h]=obj.calc(a,b,c,n);`获取三个答案，`f,g,h` 意义如下：
   
   $f=\sum_{i=0}^n \lfloor \frac{a\times i+b}{c}\rfloor$ ；
   
   $g=\sum_{i=0}^n \lfloor \frac{a\times i+b}{c}\rfloor\times i$ ；
   
   $h=\sum_{i=0}^n \lfloor \frac{a\times i+b}{c}\rfloor^2$ 。

### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/StaticEuclidean.h"

int main() {
    uint32_t a = 12;
    uint32_t b = 7;
    uint32_t c = 25;
    uint32_t n = 10000;
    constexpr uint32_t P = 1000000007;

    uint64_t F = 0, G = 0, H = 0;
    for (int i = 0; i <= n; i++) {
        F += (uint64_t(a) * i + b) / c;
        F %= P;
        G += (uint64_t(a) * i + b) / c * i;
        G %= P;
        H += (uint64_t(a) * i + b) / c * ((uint64_t(a) * i + b) / c);
        H %= P;
    }

    auto [f, g, h] = OY::StaticEuclidean<P>::calc(a, b, c, n);
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

