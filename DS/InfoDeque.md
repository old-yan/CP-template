### 一、模板类别

​	数据结构：带有信息统计的双向队列。

​	练习题目：

1. [B3656 【模板】双端队列 1](https://www.luogu.com.cn/problem/B3656)
2. [P8496 [NOI2022] 众数](https://www.luogu.com.cn/problem/P8496)
3. [Deque Operate All Composite](https://judge.yosupo.jp/problem/deque_operate_all_composite)(https://github.com/yosupo06/library-checker-problems/issues/815)


### 二、模板功能

​		本模板可以作为 `std::deque` 的替代品。区别是，本模板可以通过 `query_all`方法返回队列中所有元素的信息聚合值。基于不同的适配器 `Adapter` ，可以构造出不同场景下的队列。

​		 `VectorInfoDeque` 使用 `std::vector` 作为底层，性能中规中矩，在程序中可以同时开多个 `Deque` 。

​		 `GlobalInfoDeque` 使用静态数组作为底层，性能最好，但是在程序中同一时刻不能存在多个同类的 `Deque` 。

​		当模板参数 `Operation` 为 `INFODEQUE::Ignore` 时，本模板不提供信息聚合功能，可以完全作为 `std::deque` 的初始空间很小的替代。


### 三、模板示例

```c++
#include "DS/InfoDeque.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorInfoDeque<int, std::plus<int>> q;
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
    
    for (int i = 0; i < q.size(); i++) cout << q[i] << " \n"[i + 1 == q.size()];
}

int main() {
    test();
}
```

```
#输出如下
sum = 1000
sum = 1500
500 100 300 600

```

