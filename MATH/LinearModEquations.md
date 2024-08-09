### 一、模板类别

​	数学：线性同余方程组。

​	练习题目：

1. [P4774 [NOI2018] 屠龙勇士](https://www.luogu.com.cn/problem/P4774)


### 二、模板功能

​	本模板可以解决线性同余方程组问题，包括韩信点兵问题（也称孙子算经问题）。添加若干组乘数、除数和对应的余数，可以计算出最小的可能的原数值。

​	本模板与 `ChineseRemainderTheorem_ex` 的区别是，本模板处理的问题，在未知数旁边还带有一个额外的乘数。

​	 `add` 方法可以添加一组乘数、除数和对应的余数，返回一个布尔值，表示是否有矛盾。若返回 `true` ，表示添加顺利；若返回 `false` ，表示添加的乘数、除数、余数与之前的添加情况有矛盾，不存在合理的答案。

​	`query` 方法用以查询最小的符合要求的正的数值，以及符合要求的数值重复出现的周期。


### 三、模板示例

```c++
#include "IO/FastIO.h"
#include "MATH/LinearModEquations.h"

int main() {
    // 假设求解以下方程组
    // 3 * x = 7 mod 10
    // 6 * x = 5 mod 13
    OY::LinearModEquations lme;
    lme.add_equation(3, 7, 10);
    lme.add_equation(6, 5, 13);

    auto res = lme.query();
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
first result = 29
second result = 159
third result = 289

```

