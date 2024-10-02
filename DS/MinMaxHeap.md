### 一、模板类别

​	数据结构：大小根堆。

​	练习题目：

1. [Double-Ended Priority Queue](https://judge.yosupo.jp/problem/double_ended_priority_queue)(https://github.com/yosupo06/library-checker-problems/issues/874)



### 二、模板功能

​		本模板为支持同时维护最大值和最小值的堆。

​		在接口上，并无特殊之处。


### 三、模板示例

```c++
#include "DS/MinMaxHeap.h"
#include "IO/FastIO.h"

void test() {
    OY::MMHeap::Heap<int> S;
    S.push(100);
    S.push(300);
    S.push(50);
    S.push(600);
    S.push(250);
    cout << "min = " << S.top_min() << endl;
    S.pop_min();
    cout << "max = " << S.top_max() << endl;
    S.pop_max();
    cout << "min = " << S.top_min() << endl;
    S.pop_min();
    cout << "max = " << S.top_max() << endl;
    S.pop_max();
}

int main() {
    test();
}
```

```
#输出如下
min = 50
max = 600
min = 100
max = 300

```

