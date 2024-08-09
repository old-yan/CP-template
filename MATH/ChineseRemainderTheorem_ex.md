### 一、模板类别

​	数学：扩展中国剩余定理。

​	练习题目：

1. [P4777 【模板】扩展中国剩余定理（EXCRT）](https://www.luogu.com.cn/problem/P4777)

### 二、模板功能

​	本模板可以解决韩信点兵问题（也称孙子算经问题）。添加若干组除数和对应的余数，可以计算出最小的可能的原数值。

​	本模板可以解决除数不互质的情况。

​	 `add` 方法可以添加一组除数和对应的余数，返回一个布尔值，表示是否有矛盾。若返回 `true` ，表示添加顺利；若返回 `false` ，表示添加的除数、余数与之前的添加情况有矛盾，不存在合理的答案。

​	`query` 方法用以查询最小的符合要求的正的数值，以及符合要求的数值重复出现的周期。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/ChineseRemainderTheorem_ex.h"

int main() {
    // 《孙子算经》上有名的“孙子问题”
    //  原来的题目是：“今有物不知其数，三三数之剩二，五五数之剩三，七七数之剩二。问物几何？”
    OY::ChineseRemainderTheorem_ex crt;
    crt.add(2, 3);
    crt.add(3, 5);
    crt.add(2, 7);

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

    // 如果有数据合法性的问题，就检测 add 的返回值
    // “今有物不知其数，三三数之剩二，六六数之剩一。问物几何？”
    OY::ChineseRemainderTheorem_ex crt2;
    bool valid = true;
    if (!crt2.add(2, 3)) valid = false;
    if (!crt2.add(1, 6)) valid = false;
    if (valid) {
        cout << "min result = " << crt2.query().m_rem << endl;
    } else {
        cout << "Impossible!\n";
    }
}
```

```
#输出如下
first result = 23
second result = 128
third result = 233
Impossible!

```

