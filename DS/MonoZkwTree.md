### 一、模板类别

​	数据结构： `MonoZkw` 线段树。

​	练习题目：

1. [#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
2. [#131. 树状数组 2 ：区间修改，单点查询](https://loj.ac/p/131)
3. [P1198 [JSOI2008] 最大数](https://www.luogu.com.cn/problem/P1198)
4. [P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)
5. [P1890 gcd区间](https://www.luogu.com.cn/problem/P1890)
6. [P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)
7. [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
8. [U311262 求区间后继](https://www.luogu.com.cn/problem/U311262)
9. [Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite)(https://github.com/yosupo06/library-checker-problems/issues/122)
10. [Vertex Set Path Composite](https://judge.yosupo.jp/problem/vertex_set_path_composite)(https://github.com/yosupo06/library-checker-problems/issues/190)

### 二、模板功能


​		本模板为 `ZkwTree` 在只有单点修改、没有区间修改情况下的特化。

​		本模板要求提供模板参数 `typename Tp` 元素类型； `typename IdentityMapping` 函数返回幺元； `typename Operation`  函数返回两个元素的结合结果。

​		为了方便使用，本模板提供了 `MinTree` ， `MaxTree` ， `GcdTree` ， `LcmTree` ， `BitAndTree` ， `BitOrTree` ， `SumTree` 七种特化形式，需要填写的参数更少。


### 三、模板示例

```c++
#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

void test() {
    // MonoZkw 和 Zkw 在只有单点修改情况下是完全一样的，所以相同之处不再展示
    // 展示一个单点修改的求乘积树的写法
    int A[] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
#if CPP_STANDARD >= 202002L
    // 幺元为 1
    OY::MONOZKW::Tree<uint32_t, decltype([] { return 1; }), std::multiplies<uint32_t>> S(A, A + 9);
    cout << S << endl;
    S.modify(3, 100);
    cout << S << endl;
    cout << "prod = " << S.query_all() << endl;
#else
    struct One {
        uint32_t operator()() const { return 1; }
    };
    OY::MONOZKW::Tree<uint32_t, One, std::multiplies<uint32_t>> S(A, A + 9);
    cout << S << endl;
    S.modify(3, 100);
    cout << S << endl;
    cout << "prod = " << S.query_all() << endl;
#endif
}

int main() {
    test();
}
```

```
#输出如下
[4, 9, 2, 3, 5, 7, 8, 1, 6]
[4, 9, 2, 100, 5, 7, 8, 1, 6]
prod = 12096000

```

