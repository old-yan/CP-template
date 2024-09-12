### 一、模板类别

​	数据结构： `MonoZkw` 线段树。

​	练习题目：

1. [Luck and Love](https://acm.hdu.edu.cn/showproblem.php?pid=1823)
2. [Mosaic](https://acm.hdu.edu.cn/showproblem.php?pid=4819)
3. [#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
4. [#131. 树状数组 2 ：区间修改，单点查询](https://loj.ac/p/131)
5. [P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)
6. [P1890 gcd区间](https://www.luogu.com.cn/problem/P1890)
7. [P3368 【模板】树状数组 2](https://www.luogu.com.cn/problem/P3368)
8. [P3374 【模板】树状数组 1](https://www.luogu.com.cn/problem/P3374)
9. [P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)
10. [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
11. [P4556 [Vani有约会] 雨天的尾巴 /【模板】线段树合并](https://www.luogu.com.cn/problem/P4556)
12. [U311262 求区间后继](https://www.luogu.com.cn/problem/U311262)
13. [Vertex Set Path Composite](https://judge.yosupo.jp/problem/vertex_set_path_composite)(https://github.com/yosupo06/library-checker-problems/issues/190)

### 二、模板功能


​		本模板为 `ZkwTree` 在只有单点修改、没有区间修改情况下的特化。



​		本模板通过模板参数 `typename Monoid` 确定幺半群。幺半群须满足以下要求：

1. 声明 `value_type` 为值类型；

2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；

3. 定义静态函数 `identity` ，无输入参数，返回幺元。

    本模板要求区间操作函数的运算符满足**结合律**。

​		为了方便使用，本模板提供了 `MinTree` ， `MaxTree` ， `GcdTree` ， `LcmTree` ， `BitAndTree` ， `BitOrTree` ， `BitXorTree` ， `SumTree` 八种特化形式，需要填写的参数更少。


### 三、模板示例

```c++
#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

void test() {
    // 模板内置了 Min Max Gcd Lcm BitAnd BitOr BitXor Sum 八种特化
    // 这些运算可以拿来就用
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    OY::MonoMaxTree<int> s_max(A, A + 10);
    cout << s_max << endl;
    cout << "max(A[2~6]) = " << s_max.query(2, 6) << endl;

    OY::MonoSumTree<int> s_sum(A, A + 10);
    cout << s_sum << endl;
    cout << "sum(A[2~6]) = " << s_sum.query(2, 6) << endl
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
    auto s_longest = OY::MONOZKW::Tree<GetLongest>(5, [&](int i) { return s[i]; });
    cout << s_longest << endl;
    cout << "longest is " << s_longest.query_all() << endl;

    // 自带的二分要比自己手写二分快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = s_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
    cout << "right = " << right << '\n';
}

int main() {
    test();
    test_monoid();
}
```

```
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[2~6]) = 12
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
sum(A[2~6]) = 39

[hello, cat, world, dajiahao, ok]
longest is dajiahao
right = 2

```

