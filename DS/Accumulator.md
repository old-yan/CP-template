### 一、模板类别

​	数据结构：累加器。

​	（这是自取的名字，该数据结构用来维护前缀和、后缀和，并提供暴力统计区间和的功能。）

​	练习题目：

1. [U214300 前缀最大值之和](https://www.luogu.com.cn/problem/U214300)

### 二、模板功能

#### 1.建立累加器

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标、区间下标的变量类型。

   模板参数 `typename SemiGroup` ，表示半群。

   模板参数 `bool Prefix` ，表示累加器是否存储区间前缀和。

   模板参数 `bool Suffix` ，表示累加器是否存储区间后缀和。

   构造参数 `size_type length` ，表示累加器的覆盖范围为 `[0, length)` 。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   累加器处理的问题为区间信息查询，可以是区间最大值、区间最小值，也可以是区间按位或、区间按位与、区间最大公约数以及加法、乘法、按位异或等各种运算。

   累加器并不适合进行动态区间的维护，单点修改的时间复杂度很高。但是本模板仍然支持该方法。
   
   不同于之前版本的累加器设计，目前的累加器的大量细节都放到了模板参数 `SemiGroup` 中，只需要设计好 `SemiGroup` 即可让累加器工作。
   
   对于累加器来说，半群须满足以下要求：
   
   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `op` ，接受两个 `value_type` 参数，返回它们的聚合值；
   
   累加器要求区间操作函数的运算符满足**结合律**。
   
   **注意：**
   
   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。

#### 2.建立累加器

1. 数据类型

   构造参数 `Iterator first`​ ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   使用迭代器进行初始化，可以将区间初状态直接赋到累加器里。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示累加器要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到累加器里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，保证下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间查询的区间头。

   输入参数 `Iterator last` ，表示区间查询的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   使用迭代器进行重置，可以将区间初状态直接赋到累加器里。

#### 5.单点修改(modify)

1. 数据类型

   输入参数 `size_type i` ，表示要修改的点的下标。

   输入参数 `value_type val` ，表示要修改为的值。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.查询前缀和(prefix)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的前缀区间的尾下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数只有在 `Prefix==true` 的情况下适用。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.查询后缀和(suffix)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的后缀区间的头下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数只有在 `Suffix==true` 的情况下适用。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 8.单点查询(query)

1. 数据类型

   输入参数 `size_type i` ，表示要查询的点的下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 9.区间查询(query)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的区间的左边界下标。

   输入参数 `size_type right` ，表示要查询的区间的右边界下标。（闭区间）

2. 时间复杂度

   $O(n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 10.查询全部(query_all)

1. 数据类型

2. 时间复杂度

   $O(1\sim n)$ 。
   
3. 备注

   本函数在不同的情况下采用不同的策略。


#### 11.二分查询右边界(max_right)

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

#### 12.二分查询左边界(min_left)

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
#include "DS/Accumulator.h"
#include "IO/FastIO.h"
#include "TEST/std_gcd_lcm.h"

void test() {
    // 先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};

    // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    // 建立一个区间最大值累加器
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    auto acc_max = OY::make_Accumulator<true, true>(A, A + 10, mymax);
    cout << acc_max << endl;
    cout << "max(A[3~6])     =" << acc_max.query(3, 6) << endl;
#else
    struct {
        int operator()(int x, int y) const { return x > y ? x : y; }
    } mymax;
    auto acc_max = OY::make_Accumulator<true, true>(A, A + 10, mymax);
    cout << acc_max << endl;
    cout << "max(A[3~6])     =" << acc_max.query(3, 6) << endl;
#endif

    // 建立一个区间最大公约数累加器
    struct Gcd {
        int operator()(int x, int y) const { return std::gcd(x, y); }
    };
    auto acc_gcd = OY::make_Accumulator<true, true>(A, A + 10, Gcd());
    cout << "gcd(A[3~6])     =" << acc_gcd.query(3, 6) << endl;

    // 建立一个区间按位与累加器
    // 按位与的函数类具有默认构造，可以忽略构造参数
    auto acc_bit_and = OY::make_Accumulator<true, true>(A, A + 10, std::bit_and<int>());
    cout << "bit_and(A[3~6]) =" << acc_bit_and.query(3, 6) << endl;

    // 最普通的加法累加器
    auto acc_sum = OY::make_Accumulator<true, true>(A, A + 10, std::plus<int>());
    cout << acc_sum << endl;

    // 查找从下标 1 开始总和小于等于 14 的最远边界
    auto right = acc_sum.max_right(1, [](int s) { return s <= 14; });
    cout << "right = " << right << '\n';
}

void test_semigroup() {
    // 怎样设计一个维护区间乘积的半群
    struct SemiGroup {
        using value_type = int;
        static value_type op(value_type x, value_type y) { return x * y; }
    };
    OY::PreSufTable<SemiGroup> S(10, [&](int i) { return i + 1; });
    cout << S << endl;
    cout << "prod(S[2~4]) = " << S.query(2, 4) << endl;
}

int main() {
    test();
    test_semigroup();
}
```

```
#输出如下
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
max(A[3~6])     =12
gcd(A[3~6])     =2
bit_and(A[3~6]) =0
[11, 5, 9, 12, 8, 4, 6, 15, 7, 7]
right = 2
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
prod(S[2~4]) = 60

```

