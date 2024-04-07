### 一、模板类别

​	数据结构：根树。

​	练习题目：

1. [P1890 gcd区间](https://www.luogu.com.cn/problem/P1890)
2. [P2495 [SDOI2011] 消耗战](https://www.luogu.com.cn/problem/P2495)
3. [P3128 [USACO15DEC] Max Flow P](https://www.luogu.com.cn/problem/P3128)
4. [P3258 [JLOI2014] 松鼠的新家](https://www.luogu.com.cn/problem/P3258)
5. [P3379 【模板】最近公共祖先（LCA）](https://www.luogu.com.cn/problem/P3379)
6. [P3793 由乃救爷爷](https://www.luogu.com.cn/problem/P3793)
7. [P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)
8. [P4103 [HEOI2014] 大工程](https://www.luogu.com.cn/problem/P4103)


### 二、模板功能

#### 1.建立根数

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Node` ，表示结点类型。

   模板参数 `size_type MAX_NODE` ，表示最大结点数。

   模板参数 `typename InterTable` ，表示用于块间区间维护的类，推荐使用 `OY::Cat::Table<Node, MAX_NODE>` 。

   构造参数 `size_type length` ，表示根树的覆盖范围为 `[0, length)`。默认值为 `0` 。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。默认为 `Sqrt::Ignore` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   $O(n)$  ；相当于前缀后缀处理，外加 `InterTable` 执行一个 $\sqrt n$ 规模的初始化。

3. 备注

   根树处理的问题为区间的维护。具体维护的性质，与模板参数中的 `InnerTable` 密切相关。
   
    `InterTable` 设为 `ST` 表或者猫树乃至 `Zkw` 线段树均可，当把 `InterTable` 设为 `ST` 表时，根树和 `ST` 表一样只能处理满足**可重复贡献**性质的区间性质。当设为猫树或者 `Zkw` 线段树时，根树自然可以处理求和、乘积等问题。

   不同于之前版本的根树设计，目前的根树的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让根树工作。

   对于根树来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值。

   至此，根树所需的结点功能已经足够。此外，结点仍需要满足 `InterTable` 对结点的要求。

   一般的，我们称 `op` 函数执行的是聚合操作。

   根树要求区间操作函数的运算符满足**结合律**。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。默认情况下， `mapping` 为 `Sqrt::Ignore` 类，表示不进行初始化，比如要建立一颗空的最大值根树，由于全局变量值本身就是零，所以无需进行初始化。

   **注意：**

   本数据结构的各种操作的时间复杂度，与 `InterTable` 密切相关。姑且按照 `InterTable` 为 `OY::Cat::Table<Node>` 来填写时间复杂度；同时会注明使用其他参数时的复杂度变化。

#### 2.建立根树

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到根树里。


#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示根树要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   $O(n)$  ；相当于前缀后缀处理，外加 `InterTable` 执行一个 $\sqrt n$ 规模的初始化。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到根树里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到根树里。


#### 5.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i​` ，表示单点赋值的下标。

   输入参数 `const value_type &val​` ，表示赋的值。

2. 时间复杂度

   $O(\sqrt n)$ ；相当于前缀后缀表对一个单元进行更新，然后 `InterTable` 进行一次单点赋。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.单点增值(add)

1. 数据类型

   输入参数 `size_type i` ，表示单点增值的下标。

   输入参数 `const value_type &inc​` ，表示增量大小。

2. 时间复杂度

   $O(\sqrt n)$ ；相当于前缀后缀表对一个单元进行更新，然后 `InterTable` 进行一次单点赋。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.单点查询(query)

1. 数据类型

   输入参数 `size_type i` ，表示查询的下标。

2. 时间复杂度

    $O(1)$ ；相当于 `InnerTable` 执行一次单点查询。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 8.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   均摊 $O(1)$ ；相当于 `InterTable` 进行一次区间查询，而 `InnerTable` 执行一次前缀查询和一次后缀查询。
   
   最大 $O(\sqrt n)$ ；相当于在原数据表的一个单元内进行暴力累积。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 9.查询全部(query_all)

1. 数据类型

2. 时间复杂度

    $O(1)$ ；相当于 `InterTable` 进行一次区间查询。

#### 10.树上二分查询右边界(max_right)

1. 数据类型

   输入参数 `size_type left` ，表示左边界。

   输入参数 `Judger &&judge` ，表示需要满足的判断条件。

   返回类型 `size_type` ，表示在满足条件情况下的最大右边界。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   假设本函数返回 `r` ，则表示，对于 `i∈[left, r]`  ，均有 `judge(query(left, i))` 为真。而当 `i>r` 时，有 `judge(query(left, i))` 为假。显然，`r` 的最大值为 `m_size-1` 。

   如果从 `left` 开始，即使长度为一的区间也不能满足判断条件，那么返回 `left-1`  。所以 `r` 的最小值为 `left-1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 11.树上二分查询左边界(min_left)

1. 数据类型

   输入参数 `size_type right` ，表示右边界。

   输入参数 `Judger &&judge` ，表示需要满足的判断条件。

   返回类型 `size_type` ，表示在满足条件情况下的最小左边界。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   假设本函数返回 `l` ，则表示，对于 `i∈[l, right]`  ，均有 `judge(query(i, right))` 为真。而当 `i<l` 时，有 `judge(query(i, right))` 为假。显然，`l` 的最小值为 `0` 。

   如果从 `right` 开始往左走，即使长度为一的区间也不能满足判断条件，那么返回 `right+1`  。所以 `l` 的最大值为 `right+1` 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


### 三、模板示例

```c++
#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};

    // 建立一个区间最大值根树
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    auto sqrt_max = OY::make_SqrtTree<1000>(A, A + 10, mymax);
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } mymax;
    auto sqrt_max = OY::make_SqrtTree<1000>(A, A + 10, mymax);
#endif
    cout << sqrt_max << endl;
    cout << "max(A[3~6])     =" << sqrt_max.query(3, 6) << endl;

    // 建立一个区间最小值根树
    // 甚至可以适用 stl 的最值函数
    auto sqrt_min = OY::make_SqrtTree<1000>(A, A + 10, std::min);
    cout << "min(A[3~6])     =" << sqrt_min.query(3, 6) << endl;

    // 建立一个区间最大公约数根树
    // 可以在参数框里写 lambda
    auto sqrt_gcd = OY::make_SqrtTree<1000>(A, A + 10, std::gcd);
    cout << "gcd(A[3~6])     =" << sqrt_gcd.query(3, 6) << endl;

    // 建立一个区间按位与根树
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto sqrt_bit_and = OY::make_SqrtTree<1000>(A, A + 10, std::bit_and<int>());
    cout << "bit_and(A[3~6]) =" << sqrt_bit_and.query(3, 6) << endl;

    // 建立一个区间按位或根树
    // 一开始可以是空的
    auto sqrt_bit_or = OY::make_SqrtTree<int, 1000>(0, std::bit_or<int>());
    sqrt_bit_or.reset(A, A + 10);
    cout << "bit_or(A[3~6])  =" << sqrt_bit_or.query(3, 6) << endl;

    // 便利化措施：由于实际使用的时候，往往是最值较多，所以最大值最小值有特化
    auto sqrt_default = OY::SqrtMaxTable<int, 1000>();
    sqrt_default.reset(A, A + 10);
    cout << "max(A[0~9])     =" << sqrt_default.query(0, 9) << endl;

    auto sqrt_default2 = OY::SqrtMinTable<int, 1000>();
    sqrt_default2.reset(A, A + 10);
    cout << "min(A[0~9])     =" << sqrt_default2.query(0, 9) << endl;

    // 通过比较函数的重载，实现各种意义上的取最值
    struct Cmp {
        bool operator()(const std::string &x, const std::string &y) const {
            return x.size() < y.size();
        }
    };
    std::vector<std::string> ss{"hello", "cat", "world", "dajiahao", "ok"};
    auto sqrt_longest = OY::Sqrt::Table<OY::Sqrt::BaseNode<std::string, Cmp>, 1 << 10>(5);
    for (int i = 0; i < 5; i++) {
        sqrt_longest.modify(i, ss[i]);
    }
    cout << sqrt_longest << endl;
    cout << "longest is " << sqrt_longest.query_all() << endl;

    // 自带的二分要比自己手写二分略快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = sqrt_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
    cout << "right = " << right << '\n';

    // 做个性能测试，来个很大的表，全部单点更新一遍，看会不会超时
    // 显然没超时，所以根树单点修改也蛮快，注意 add 要比 modify 更快
    auto sqrt_sum = OY::make_SqrtTree<uint64_t, 1500000>(500000, std::plus<uint64_t>(), [](uint32_t i) { return i; });
    cout << sqrt_sum.query_all() << '\n';

    for (uint32_t i = 0; i < sqrt_sum.m_size; i++) sqrt_sum.add(i, 1);
    cout << sqrt_sum.query_all() << '\n';

    for (uint32_t i = 0; i < sqrt_sum.m_size; i++) sqrt_sum.modify(i, sqrt_sum.query(i) + 1);
    cout << sqrt_sum.query_all() << '\n';
}
```

```
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[3~6])     =12
min(A[3~6])     =4
gcd(A[3~6])     =2
bit_and(A[3~6]) =0
bit_or(A[3~6])  =14
max(A[0~9])     =15
min(A[0~9])     =4
[hello, cat, world, dajiahao, ok]
longest is dajiahao
right = 2
124999750000
125000250000
125000750000

```

