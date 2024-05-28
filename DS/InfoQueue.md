### 一、模板类别

​	数据结构：带有信息统计的队列。

​	练习题目：

1. [Queue Operate All Composite](https://judge.yosupo.jp/problem/queue_operate_all_composite)(https://github.com/yosupo06/library-checker-problems/issues/208)

### 二、模板功能

​		本模板可以作为 `std::queue` 的替代品。区别是，本模板可以通过 `query_all`方法返回队列中所有元素的信息聚合值。基于不同的适配器 `Adapter` ，可以构造出不同场景下的队列。

​		 `VectorInfoQueue` 使用 `std::vector` 作为底层，性能中规中矩，在程序中可以同时开多个 `Queue` 。

​		 `GlobalInfoQueue` 使用静态数组作为底层，性能最好，但是在程序中同一时刻不能存在多个同类的 `Queue` 。

​		当模板参数 `Operation` 为 `INFOQUEUE::Ignore` 时，本模板不提供信息聚合功能，可以完全作为 `std::deque` 的初始空间很小的替代。


### 三、模板示例

```c++
#include "DS/InfoQueue.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorInfoQueue<int, std::plus<int>> q;
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
    auto get_max = [](int x, int y) { return std::max(x, y); };
    OY::GlobalInfoQueue<int, decltype(get_max), 1000> q(get_max);
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

int main() {
    test();
    test_max();
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

```

