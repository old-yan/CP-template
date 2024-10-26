### 一、模板类别

​	数据结构： `Mono` 配对堆。

​	练习题目：

1. [P1801 黑匣子](https://www.luogu.com.cn/problem/P1801)
2. [P3377 【模板】左偏树（可并堆）](https://www.luogu.com.cn/problem/P3377)
3. [P3378 【模板】堆](https://www.luogu.com.cn/problem/P3378)
4. [U361730 【模板】完全体·堆](https://www.luogu.com.cn/problem/U361730)
5. [旅途的终点](https://ac.nowcoder.com/acm/problem/275989)


### 二、模板功能

​		本模板为 `PairHeap` 在只有信息查询、没有信息修改情况下的特化。

​		本模板通过模板参数 `typename CommutativeMonoid` 确定交换幺半群。交换幺半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 声明 `sum_type` 为值类型，且可以和 `value_type` 双向类型转换。若无此声明，默认等同于 `value_type` ；

3. 定义静态函数 `op` ，接受两个 `sum_type` 参数，返回它们的聚合值；

4. 定义静态函数 `identity` ，无输入参数，返回幺元。


​		幺半群的 `value_type` 不可以为 `void` 。

​		本模板要求区间操作函数的运算符满足**结合律** 和**交换律**。

​		为了方便使用，本模板提供了 `MinHeap` ， `MaxHeap` ， `GcdHeap` ， `LcmHeap` ， `BitAndHeap` ， `BitOrHeap` ， `BitXorHeap` ， `SumHeap` 八种特化形式，需要填写的参数更少。

### 三、模板示例

```c++
#include "DS/MonoPairHeap.h"
#include "IO/FastIO.h"

void test() {
    // 一个小根堆
    // 值类型 int，和类型 int64
    OY::VectorMonoSumPairHeap<int, int64_t, 0, std::greater<int>> Q;
    for (int v = 1; v <= 10; v++) Q.push(v);
    Q.pop();
    Q.pop();
    cout << Q.query_all() << endl;
}

int main() {
    test();
}
```

```
#输出如下
52

```

