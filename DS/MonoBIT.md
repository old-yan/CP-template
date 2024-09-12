### 一、模板类别

​	数据结构： `Mono` 树状数组。

​	练习题目：

1. [#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
2. [#131. 树状数组 2 ：区间修改，单点查询](https://loj.ac/p/131)
3. [P3368 【模板】树状数组 2](https://www.luogu.com.cn/problem/P3368)
4. [P3374 【模板】树状数组 1](https://www.luogu.com.cn/problem/P3374)
5. [P5057 [CQOI2006] 简单题](https://www.luogu.com.cn/problem/P5057)

### 二、模板功能


​		本模板为 `BIT` 在只有单点修改、没有区间修改情况下的特化。

​		本模板与 `MonoZkw` 的区别是，本模板的单点修改不能进行单点赋值，而只能在旧值的基础上结合一个增量。

​		本模板通过模板参数 `typename CommutativeGroup` 确定交换群。交换群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

4. 定义静态函数 `inverse` ，输入参数一个 `value_type` ，返回其逆元。如果不存在本函数，则无法完成任意区间查询。

    本模板要求区间操作函数的运算符满足**结合律**和**交换律**。

​		为了方便使用，本模板提供了 `MinBIT` ， `MaxBIT` ， `GcdBIT` ， `LcmBIT` ， `BitAndBIT` ， `BitOrBIT` ， `BitXorBIT` ， `SumBIT` 八种特化形式，需要填写的参数更少。


### 三、模板示例

```c++
#include "DS/MonoBIT.h"
#include "IO/FastIO.h"

void test() {
    // 模板内置了 Min Max Gcd Lcm BitAnd BitOr BitXor Sum 八种特化
    // 这些运算可以拿来就用
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    OY::MonoMaxBIT<int> s_max(A, A + 10);
    cout << "max(A[0~6]) = " << s_max.presum(6) << endl;

    OY::MonoSumBIT<int> s_sum(A, A + 10);
    cout << "sum(A[0~6]) = " << s_sum.presum(6) << endl
         << endl;
}

void test_monoid() {
    // 通过半群的重写，实现各种意义上的取最值
    struct GetLongest {
        using value_type = std::string;
        static value_type op(const std::string &x, const std::string &y) {
            return x.size() > y.size() ? x : y;
        }
        static std::string identity() { return ""; }
    };
    std::vector<std::string> s{"hello", "cat", "world", "dajiahao", "ok"};
    auto s_longest = OY::MONOBIT::Tree<GetLongest>(5, [&](int i) { return s[i]; });
    cout << "longest is " << s_longest.query_all() << endl;

    // 自带的二分要比自己手写二分快
    // 查找从下标 0 开始字符串长度在 5 以内的最远边界
    auto right = s_longest.max_right([](const std::string &s) { return s.size() <= 5; });
    cout << "right = " << right << '\n';
}

int main() {
    test();
    test_monoid();
}
```

```
#输出如下
max(A[0~6]) = 12
sum(A[0~6]) = 55

longest is dajiahao
right = 2

```

