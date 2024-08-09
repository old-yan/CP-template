### 一、模板类别

​	数学：中国剩余定理。

​	练习题目：

1. [P1495 【模板】中国剩余定理（CRT）/ 曹冲养猪](https://www.luogu.com.cn/problem/P1495)

### 二、模板功能

​	本模板可以解决韩信点兵问题（也称孙子算经问题）。添加若干组除数和对应的余数，可以计算出最小的可能的原数值。

​	本模板可以解决除数之间互质的情况。

​	 `add` 方法可以添加一组除数和对应的余数。

​	 `query` 方法用以查询最小的符合要求的正的数值，以及符合要求的数值重复出现的周期。

​	本模板要求所有的除数的乘积在 `uint64_t` 的表示范围内。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem.h"

int main() {
    // 《孙子算经》上有名的“孙子问题”
    //  原来的题目是：“今有物不知其数，三三数之剩二，五五数之剩三，七七数之剩二。问物几何？”
    OY::ChineseRemainderTheorem crt(3);
    crt.add(2, 3);
    crt.add(3, 5);
    crt.add(4, 7);

    // 结果是个 pair
    auto res = crt.query();
    // 最小答案
    auto min = res.m_rem;
    // 答案的周期
    auto cycle = res.m_div;

    // min, min+cycle, min+cycle*2 ... 均为可能的答案
    cout << "first result = " << min << endl;
    cout << "second result = " << min + cycle << endl;
    cout << "third result = " << min + cycle * 2 << endl;
}
```

```
#输出如下
first result = 53
second result = 158
third result = 263

```

