### 一、模板类别

​	数据结构：带有信息统计的双向队列。

​	练习题目：

1. [B3656 【模板】双端队列 1](https://www.luogu.com.cn/problem/B3656)
2. [P8496 [NOI2022] 众数](https://www.luogu.com.cn/problem/P8496)
3. [Deque Operate All Composite](https://judge.yosupo.jp/problem/deque_operate_all_composite)(https://github.com/yosupo06/library-checker-problems/issues/815)


### 二、模板功能

​		本模板可以作为 `std::deque` 的替代品。区别是，本模板可以通过 `query_all`方法返回队列中所有元素的信息聚合值。基于不同的适配器 `Container` ，可以构造出不同场景下的队列。

​		 `VectorDeque` 使用 `std::vector` 作为底层，性能中规中矩，在程序中可以同时开多个 `Deque` 。

​		 `GlobalDeque` 使用静态数组作为底层，性能最好，但是在程序中同一时刻不能存在多个同类的 `Deque` 。

​		本模板通过模板参数 `typename SemiGroup` 确定半群。半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 声明 `sum_type` 为值类型，且可以和 `value_type` 双向类型转换。若无此声明，默认等同于 `value_type` ；

3. 定义静态函数 `op` ，接受两个 `sum_type` 参数，返回它们的聚合值；

​		半群的 `value_type` 不可以为 `void` 。

​		可以没有 `op` 函数。此时本模板仅模拟普通队列，不维护半群信息。

​		本模板要求区间操作函数的运算符满足**结合律**。

​		为了方便使用，本模板提供了 `MinDeque` ， `MaxDeque` ， `GcdDeque` ， `LcmDeque` ， `BitAndDeque` ， `BitOrDeque` ， `BitXorDeque` ， `SumDeque` 八种特化形式，需要填写的参数更少。



### 三、模板示例

```c++
#include "DS/Deque.h"
#include "IO/FastIO.h"

void test() {
    OY::SumDeque<int> q;
    q.push_back(100);
    q.push_back(300);
    q.push_back(200);
    q.push_front(400);
    cout << "sum = " << q.query_all() << endl;

    q.pop_front();
    q.pop_back();
    q.push_front(500);
    q.push_back(600);
    cout << "sum = " << q.query_all() << endl;

    cout << q << endl;
}

int main() {
    test();
}
```

```
#输出如下
sum = 1000
sum = 1500
[500, 100, 300, 600]

```

