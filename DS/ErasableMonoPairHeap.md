### 一、模板类别

​	数据结构：可删除元素的配对堆。

​	练习题目：

1. [3321. 计算子数组的 x-sum II](https://leetcode.cn/problems/find-x-sum-of-all-k-long-subarrays-ii)


### 二、模板功能

​		本模板为支持维护信息的堆。同时，借助懒删除技术，实现了堆中元素的删除。

​		与 `MonoPairHeap` 不同的是，本模板要求传入的代数结构为交换群，需要存在逆元。只有通过差分才可以实时维护堆的信息。

​		需要注意的是， `erase` 函数需要保证，每次传入的元素必须仍在堆中，未被删除。

​		在接口上，并无特殊之处。


### 三、模板示例

```c++
#include "DS/ErasableMonoPairHeap.h"
#include "IO/FastIO.h"

void test() {
    OY::VectorErasableMonoSumPairHeap<int> S;
    S.push(100);
    S.push(400);
    S.push(200);
    S.push(300);
    S.push(500);
    S.erase(200);
    S.erase(400);
    cout << "size = " << S.size() << endl;
    S.pop();
    cout << "sum = " << S.query_all() << endl;
}

int main() {
    test();
}
```

```
#输出如下
size = 3
sum = 400

```

