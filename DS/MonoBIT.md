### 一、模板类别

​	数据结构： `Mono` 树状数组。

​	练习题目：

1. [P5057 [CQOI2006] 简单题](https://www.luogu.com.cn/problem/P5057)

### 二、模板功能


​		本模板为 `BIT` 在只有单点修改、没有区间修改情况下的特化。

​		本模板与 `MonoZkw` 的区别是，本模板的单点修改不能进行单点赋值，而只能在旧值的基础上结合一个增量。

​		本模板要求提供模板参数 `typename Tp` 元素类型； `typename IdentityMapping` 函数返回幺元； `typename Operation`  函数返回两个元素的结合结果。

​		为了方便使用，本模板提供了 `MinBIT` ， `MaxBIT` ， `GcdBIT` ， `LcmBIT` ， `BitAndBIT` ， `BitOrBIT` ， `BitXorBIT` ， `SumBIT` 八种特化形式，需要填写的参数更少。


### 三、模板示例

```c++
#include "DS/MonoBIT.h"
#include "IO/FastIO.h"

void test() {
    // MonoBIT 和 BIT 在只有单点修改情况下是完全一样的，所以相同之处不再展示
    // 展示一个单点修改的求乘积树的写法
    int A[] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
#if CPP_STANDARD >= 202002L
    // 幺元为 1
    OY::MONOBIT::Tree<uint32_t, decltype([] { return 1; }), std::multiplies<uint32_t>> S(A, A + 9);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
    S.add(3, 100);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
#else
    struct One {
        uint32_t operator()() const { return 1; }
    };
    OY::MONOBIT::Tree<uint32_t, One, std::multiplies<uint32_t>> S(A, A + 9);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
    S.add(3, 100);
    cout << "prod(A[0~2]) = " << S.presum(2) << endl;
    cout << "prod(A[0~3]) = " << S.presum(3) << endl;
#endif
}

int main() {
    test();
}
```

```
#输出如下
prod(A[0~2]) = 72
prod(A[0~3]) = 216
prod(A[0~2]) = 72
prod(A[0~3]) = 21600

```

