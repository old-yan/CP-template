### 一、模板类别

​	数据结构： `ST` 表。

​	练习题目：

1. [P1886 滑动窗口 /【模板】单调队列](https://www.luogu.com.cn/problem/P1886)
2. [P1890 gcd区间](https://www.luogu.com.cn/problem/P1890)
3. [P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)

### 二、模板功能

#### 1.建立ST表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Node` ，表示结点类型。

   模板参数 `size_type MAX_NODE` ，表示最大结点数，默认为 `1<<22` 。

   构造参数 `size_type length` ，表示 `ST` 表的覆盖范围为 `[0, length)`。默认值为 `0` 。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。默认为 `ST::Ignore` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

    `ST` 表处理的问题为区间最值查询，可以是区间最大值、区间最小值，也可以是区间按位或、区间按位与、区间最大公约数。本质上，按位或就是二进制的每位最大值，按位与就是二进制的每位最小值。只要满足**可重复贡献**的性质，就可以使用 `ST` 表。

    **可重复贡献**：比如想查询区间 `[1,13]​` 的最大值，可以先查询 `[1,8]` 的最大值，再查询 `[6,13]` 的最大值，再将二者合并。尽管两次查询的区间有重复部分，但是不影响结果。

   不同于之前版本的 `ST` 表设计，目前的 `ST` 表的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让 `ST` 表工作。

   对于 `ST` 表来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值。

   至此， `ST` 表所需的结点功能已经足够。

   一般的，我们称 `op` 函数执行的是聚合操作。

    `ST` 表要求区间操作函数的运算符满足**结合律**。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。默认情况下， `mapping` 为 `ST::Ignore` 类，表示不进行初始化，比如要建立一颗空的最大值 `ST` 表，由于全局变量值本身就是零，所以无需进行初始化。

#### 2.建立ST表

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到 `ST` 表里。


#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示 `ST` 表要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到 `ST` 表里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到 `ST` 表里。


#### 5.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i​` ，表示单点赋值的下标。

   输入参数 `const value_type &val​` ，表示赋的值。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.单点增值(add)

1. 数据类型

   输入参数 `size_type i` ，表示单点增值的下标。

   输入参数 `const value_type &inc​` ，表示增量大小。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.单点查询(query)

1. 数据类型

   输入参数 `size_type i` ，表示查询的下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 8.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 9.查询全部(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

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
#include <vector>

#include "DS/STTable.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};

    // 建立一个区间最大值 ST 表
    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    auto st_max = OY::make_STTable<1000>(A, A + 10, mymax);
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } mymax;
    auto st_max = OY::make_STTable<1000>(A, A + 10, mymax);
#endif
    cout << st_max << endl;
    cout << "max(A[3~6])     =" << st_max.query(3, 6) << endl;

    // 建立一个区间最小值 ST 表
    // 甚至可以适用 stl 的最值函数
    auto st_min = OY::make_STTable<1000>(A, A + 10, std::min);
    cout << "min(A[3~6])     =" << st_min.query(3, 6) << endl;

    // 建立一个区间最大公约数 ST 表
    // 可以在参数框里写 lambda
    auto st_gcd = OY::make_STTable<1000>(A, A + 10, std::gcd);
    cout << "gcd(A[3~6])     =" << st_gcd.query(3, 6) << endl;

    // 建立一个区间按位与 ST 表
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto st_bit_and = OY::make_STTable<1000>(A, A + 10, std::bit_and<int>());
    cout << "bit_and(A[3~6]) =" << st_bit_and.query(3, 6) << endl;

    // 建立一个区间按位或 ST 表
    // 一开始可以是空的
    auto st_bit_or = OY::make_STTable<int, 1000>(0, std::bit_or<int>());
    st_bit_or.reset(A, A + 10);
    cout << "bit_or(A[3~6])  =" << st_bit_or.query(3, 6) << endl;

    // 便利化措施：由于实际使用的时候，往往是最值较多，所以最大值最小值有特化
    auto st_default = OY::STMaxTable<int, 1000>();
    st_default.reset(A, A + 10);
    cout << "max(A[0~9])     =" << st_default.query(0, 9) << endl;

    auto st_default2 = OY::STMinTable<int, 1000>();
    st_default2.reset(A, A + 10);
    cout << "min(A[0~9])     =" << st_default2.query(0, 9) << endl;

    // 通过比较函数的重载，实现各种意义上的取最值
    struct Cmp {
        bool operator()(const std::string &x, const std::string &y) const {
            return x.size() < y.size();
        }
    };
    std::vector<std::string> ss{"hello", "cat", "world", "dajiahao", "ok"};
    auto st_longest = OY::ST::Table<OY::ST::BaseNode<std::string, Cmp>, 1 << 10>(5);
    for (int i = 0; i < 5; i++) {
        st_longest.modify(i, ss[i]);
    }
    cout << st_longest << endl;
    cout << "longest is " << st_longest.query_all() << endl;

    // 自带的二分要比自己手写二分略快
    // 查找从下标 1 开始字符串长度在 5 以内的最远边界
    auto right = st_longest.max_right(1, [](const std::string &s) { return s.size() <= 5; });
    cout << "right = " << right << '\n';
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

```

