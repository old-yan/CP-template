### 一、模板类别

​	数据结构：带有信息统计的队列。

​	练习题目：

1. [#2669. 「NOI2013」快餐店](https://loj.ac/p/2669)
2. [P1399 [NOI2013] 快餐店](https://www.luogu.com.cn/problem/P1399)
3. [P10843 【MX-J2-T4】Turtle and Cycles](https://www.luogu.com.cn/problem/P10843)
4. [#126. 【NOI2013】快餐店](https://uoj.ac/problem/126)
5. [Queue Operate All Composite](https://judge.yosupo.jp/problem/queue_operate_all_composite)(https://github.com/yosupo06/library-checker-problems/issues/208)

### 二、模板功能

​		本模板可以作为 `std::queue` 的替代品。区别是，本模板可以通过 `query_all`方法返回队列中所有元素的信息聚合值。基于不同的容器 `Container` ，可以构造出不同场景下的队列。

​		 `VectorQueue` 使用 `std::vector` 作为底层，性能中规中矩，在程序中可以同时开多个 `Queue` 。

​		 `GlobalQueue` 使用静态数组作为底层，性能最好，但是在程序中同一时刻不能存在多个同类的 `Queue` 。

​		本模板通过模板参数 `typename SemiGroup` 确定半群。半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 声明 `sum_type` 为值类型，且可以和 `value_type` 双向类型转换。若无此声明，默认等同于 `value_type` ；

3. 定义静态函数 `op` ，接受两个 `sum_type` 参数，返回它们的聚合值；

​		半群的 `value_type` 不可以为 `void` 。

​		半群可以没有 `op` 函数。此时本模板仅模拟普通队列，不维护半群信息。

​		本模板要求区间操作函数的运算符满足**结合律**。

​		为了方便使用，本模板提供了 `MinQueue` ， `MaxQueue` ， `GcdQueue` ， `LcmQueue` ， `BitAndQueue` ， `BitOrQueue` ， `BitXorQueue` ， `SumQueue` 八种特化形式，需要填写的参数更少。


### 三、模板示例

```c++
#include "DS/Queue.h"
#include "IO/FastIO.h"

void test() {
    OY::SumQueue<int> q;
    q.push(100);
    q.push(300);
    q.push(200);
    cout << "sum = " << q.query_all() << endl;

    q.pop();
    cout << "front = " << q.front() << endl;
    cout << "sum = " << q.query_all() << endl;

    for (int i = 0; i < q.size(); i++) cout << q[i] << " \n"[i + 1 == q.size()];
    cout << endl;
}

void test_max() {
    OY::MaxQueue<int> q;
    q.push(100);
    q.push(30);
    q.push(20);
    cout << "max = " << q.query_all() << endl;

    q.pop();
    cout << "front = " << q.front() << endl;
    cout << "max = " << q.query_all() << endl;

    q.push(80);
    cout << "max = " << q.query_all() << endl;

    for (int i = 0; i < q.size(); i++) cout << q[i] << " \n"[i + 1 == q.size()];
    cout << endl;
}

void test_semigroup() {
    struct SemiGroup {
        using value_type = std::string;
        static value_type op(std::string x, std::string y) { return x + y; }
    };
    OY::QUE::Queue<SemiGroup> S;
    S.push("h");
    S.push("e");
    S.push("l");
    cout << S << endl;
    S.push(S.back());
    S.push("o");
    cout << S << endl;
    cout << S.query_all() << endl;
}

int main() {
    test();
    test_max();
    test_semigroup();
}
```

```
#输出如下
sum = 600
front = 300
sum = 500
300 200

max = 100
front = 30
max = 30
max = 80
30 20 80

[h, e, l]
[h, e, l, l, o]
hello

```

