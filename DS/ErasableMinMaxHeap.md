### 一、模板类别

​	数据结构：可删除元素的大小根堆。

​	练习题目：

1. [P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)



### 二、模板功能

​		本模板为支持同时维护最大值和最小值的堆。同时，借助懒删除技术，实现了堆中元素的删除。

​		需要注意的是， `erase` 函数需要保证，每次传入的元素必须仍在堆中，未被删除。

​		在接口上，并无特殊之处。


### 三、模板示例

```c++
#include "DS/ErasableMinMaxHeap.h"
#include "IO/FastIO.h"

void test() {
    OY::EMMHeap::Heap<int> S;
    S.push(100);
    S.push(400);
    S.push(200);
    S.push(300);
    S.push(500);
    S.erase(200);
    S.erase(400);
    cout << "size = " << S.size() << endl;
    S.pop_min();
    cout << "min = " << S.top_min() << endl;
}

int main() {
    test();
}
```

```
#输出如下
size = 3
min = 300

```

