### 一、模板类别

​	数据结构：小波表，小波树。

​	练习题目：

1. [Minimum Sum](https://acm.hdu.edu.cn/showproblem.php?pid=3473)
2. [2935. 找出强数对的最大异或值 II](https://leetcode.cn/problems/maximum-strong-pair-xor-ii/)
3. [P1972 [SDOI2009] HH的项链](https://www.luogu.com.cn/problem/P1972)
4. [P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)
5. [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
6. [fsl 的背包](https://ac.nowcoder.com/acm/problem/263978)


### 二、模板功能

​		本模板分为小波表与小波树两部分。 `Wavelet Tree` 向来被翻译为小波树或者波纹疾走树，但是对于 `Wavelet Matrix` 没有特有的翻译，这里我姑且称之为小波表。

​		小波表只能处理 `uint32_t` 或者 `uint64_t` 元素的区间，而小波树通过离散化的方法，支持了各种类型元素的区间。除此之外，二者并无区别。下文统称为小波树。

#### 1.建立小波树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Tp` ，表示元素类型。如果是小波表，必须为 `uint32_t` 或者 `uint64_t` 之一；如果是小波树，必须支持通过 `std::sort` 和 `std::unique` 进行离散化。
   
   模板参数 `typename SumTable` ，表示对元素进行累积的表的类型。默认为 `VoidTable` ，表示不进行累积。

   构造参数 `size_type length` ，表示小波树的覆盖范围为 `[0, length)`。默认值为 `0` 。

   构造参数 `InitMapping &&mapping` ，表示在初始化时，从下标到值的映射函数。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

   构造参数 `size_type alpha` ，表示本次构造用到的值域最大位宽。默认等于 `0` ，表示自动计算取适合的值。小波表有此参数，小波树无此参数。
   
   构造参数 `TableMapping &&table_mapping` ，表示将元素存储到 `SumTable` 中时对元素进行的映射变换。默认为 `WaveLet::Ignore` 表示不进行变换。

2. 时间复杂度

    小波表为 $O(n\cdot \alpha)$ ；小波树为 $O(n\cdot \log n)$ 。

3. 备注

   小波树处理的问题为区间最小值查询、区间最大值查询、区间某数频率查询、区间某值域的数的频率查询、区间第 `k​` 序值查询。
   
   当 `SumTable` 不为空表类型时，可以进行区间前 `k` 个最小数的累积值的查询、区间前 `k` 个最大数的累积值的查询。具体的累积内容，由表的类型决定；比如传递 `OY::AdjDiff` 模板库里的前缀和表，就可以查询区间前 `k` 个最小/最大数的和。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。

   小波树只能处理静态区间上的问题，所以没有提供修改区间的入口。

#### 2.建立小波树

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到小波树里。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示小波树要处理的区间大小。

   输入参数 `InitMapping &&mapping` ，表示初始化时，从下标到值的映射函数。

   输入参数 `size_type alpha` ，表示本次构造用到的值域最大位宽。默认等于 `0` ，表示自动计算取适合的值。小波表有此参数，小波树无此参数。
   
   输入参数 `TableMapping &&table_mapping` ，表示讲元素存储到 `SumTable` 中时对元素进行的映射变换。默认为 `WaveLet::Ignore` 表示不进行变换。

2. 时间复杂度

    小波表为 $O(n\cdot \alpha)$ ；小波树为 $O(n\cdot \log n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到小波树里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   输入参数 `size_type alpha` ，表示本次构造用到的值域最大位宽。默认等于 `0` ，表示自动计算取适合的值。小波表有此参数，小波树无此参数。
   
   输入参数 `TableMapping &&table_mapping` ，表示讲元素存储到 `SumTable` 中时对元素进行的映射变换。默认为 `WaveLet::Ignore` 表示不进行变换。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到小波树里。

#### 5.区间某数频率查询(count)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const Tp &val` ，表示要查找的元素。

   返回类型 `size_type` ，表示查询到的频率。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 6.区间某范围内数字的频率查询(count)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const Tp &minimum` ，表示要求范围的最小值。

   输入参数 `const Tp &maximum` ，表示要求范围的最大值。(闭区间)

   返回类型 `size_type` ，表示查询到的频率。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 7.区间某数排名查询(rank)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const Tp &val` ，表示要查找的元素。

   返回类型 `size_type` ，表示查询到的比其更小的元素的数目。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

   本函数等价于区间内小于 `val` 的元素数目。

   本函数没有进行参数检查，所以请自己确保下标合法。

#### 8.区间最小值查询(minimum)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `Tp` ，表示查询到的元素。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 9.区间最大值查询(maximum)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `Tp` ，表示查询到的元素。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。

#### 10.区间第k序值查询(quantile)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const size_type &k` ，表示要查找的元素的大小次序。

   返回类型 `Tp` ，表示查询到的元素。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

    `k` 的取值范围从 `0` 开始，表示**如果**将原区间的 `[l, r]` 内按照指定排序函数进行排序，那么排在 `[l + k]` 位置的元素

   本函数没有进行参数检查，所以请自己确保下标合法，且 `k​` 处于 `[0,r-l]` 范围内。

#### 11.区间最大异或匹配(max_bitxor)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `Tp val` ，表示要寻找匹配的元素。

   返回类型 `Tp` ，表示查询到的元素与 `val` 进行异或的最大结果。

2. 时间复杂度

   $O(\alpha)$ 。

3. 备注

   本方法仅适用于小波表。

   本函数没有进行参数检查，所以请自己确保下标合法。

#### 12.对区间k小数的累计值调用回调(do_for_ksmallest)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `size_type k` ，表示要查找的元素的数量。

   输入参数 `Callback &&call` ，表示对查询到的元素们的累计值调用的回调。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

    `k` 的取值范围从 `1` 开始，表示**如果**将原区间的 `[l, r]` 内按照指定排序函数进行排序，那么查询的是排在 `[l ~ l + k - 1]` 位置的元素。特别的，当 `k` 为零时，无事发生。

   本函数没有进行参数检查，所以请自己确保下标合法，且 `k​` 处于 `[0,r-l]` 范围内。
   
#### 13.对区间k大数的累计值调用回调(klargest_sum)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `size_type k` ，表示要查找的元素的数量。

   输入参数 `Callback &&call` ，表示对查询到的元素们的累计值调用的回调。

2. 时间复杂度

   小波表为 $O(\alpha)$ ；小波树为 $O(\log n)$ 。

3. 备注

    `k` 的取值范围从 `1` 开始，表示**如果**将原区间的 `[l, r]` 内按照指定排序函数进行排序，那么查询的是排在 `[r - k + 1 ~ r]` 位置的元素。特别的，当 `k` 为零时，无事发生。

   本函数没有进行参数检查，所以请自己确保下标合法，且 `k​` 处于 `[0,r-l]` 范围内。
   
### 三、模板示例

```c++
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

void test() {
    // 先给出一个长度为 10 的区间
    int A[10] = {1, 5, 6, 3, 8, 4, 4, 2, 10, 1};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个默认小波表
    auto wt = OY::WaveLet::Table<uint32_t>(A, A + 10);

    // 区间第 k
    cout << "A[3~6] No.1 = " << wt.quantile(3, 6, 0) << endl;
    cout << "A[3~6] No.2 = " << wt.quantile(3, 6, 1) << endl;
    cout << "A[3~6] No.3 = " << wt.quantile(3, 6, 2) << endl;
    cout << "A[3~6] No.4 = " << wt.quantile(3, 6, 3) << endl;

    // 区间值域统计个数
    cout << "A[0~6].count(1) = " << wt.count(0, 6, 1) << endl;
    cout << "A[0~6].count(4) = " << wt.count(0, 6, 4) << endl;
    cout << "A[0~6].count(1~4) = " << wt.count(0, 6, 1, 4) << endl;

    // 区间排名
    cout << "A[3~6] rank of (3) = " << wt.rank(3, 6, 3) << endl;
    cout << "A[3~6] rank of (4) = " << wt.rank(3, 6, 4) << endl;
    cout << "A[3~6] rank of (5) = " << wt.rank(3, 6, 5) << endl;
    cout << "A[3~6] rank of (6) = " << wt.rank(3, 6, 6) << endl;

    // 区间最小值
    cout << "A[3~6] minimum = " << wt.minimum(3, 6) << endl;

    // 区间最大值
    cout << "A[3~6] maximum = " << wt.maximum(3, 6) << endl;

    // 区间最大异或
    cout << "A[3~6].max_bitxor(9) = " << wt.max_bitxor(3, 6, 9) << endl;

    std::string B[] = {"hello", "app", "app", "world", "banana", "app", "banana", "hello"};
    // 建立一个默认小波树
    auto wt2 = OY::WaveLet::Tree<std::string>(B, B + 8);

    // 区间第 k
    cout << "B[1~6] No.1 = " << wt2.quantile(1, 6, 0) << endl;
    cout << "B[1~6] No.2 = " << wt2.quantile(1, 6, 1) << endl;
    cout << "B[1~6] No.3 = " << wt2.quantile(1, 6, 2) << endl;
    cout << "B[1~6] No.4 = " << wt2.quantile(1, 6, 3) << endl;
    cout << "B[1~6] No.5 = " << wt2.quantile(1, 6, 4) << endl;
    cout << "B[1~6] No.6 = " << wt2.quantile(1, 6, 5) << endl;

    // 区间值域统计个数
    cout << "B[1~6].count(\"app\") = " << wt2.count(1, 6, "app") << endl;
    cout << "B[1~6].count(\"yy\") = " << wt2.count(1, 6, "yy") << endl;
    cout << "B[1~6].count(\"apple\"~\"cat\") = " << wt2.count(1, 6, "apple", "cat") << endl;

    // 区间排名
    cout << "B[1~6] rank of (\"ap\")     = " << wt2.rank(1, 6, "ap") << endl;
    cout << "B[1~6] rank of (\"app\")    = " << wt2.rank(1, 6, "app") << endl;
    cout << "B[1~6] rank of (\"apple\")    = " << wt2.rank(1, 6, "apple") << endl;
    cout << "B[1~6] rank of (\"banana\") = " << wt2.rank(1, 6, "banana") << endl;

    // 区间最小值
    cout << "B[1~6] minimum = " << wt2.minimum(1, 6) << endl;

    // 区间最大值
    cout << "B[1~6] maximum = " << wt2.maximum(1, 6) << endl;
}

#include "DS/AdjDiff.h"
void test_k_sum() {
    // 借助差分模板，统计区间 k 大数的和
    int a[] = {40, 90, 20, 30, 50, 70, 80, 10, 60};
    cout << "\narr a:";
    for (int i = 0; i < 9; i++) cout << a[i] << " \n"[i == 8];

    using SumTable = OY::AdjDiff::Table<int, true>;
    OY::WaveLet::Table<uint32_t, SumTable> S(a, a + 9);

    // 统计 a[3~7] 最小的两个数的和
    auto f1 = [&](int l, int r, int k) {
        int res = 0;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_ksmallest(l, r, k, [&](int val) {
            res += val;
        });
        return res;
    };
    cout << "bottom-2 sum in a[3~7] = " << f1(3, 7, 2) << endl;

    // 统计 a[3~7] 最大的两个数的和
    auto f2 = [&](int l, int r, int k) {
        int res = 0;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_klargest(l, r, k, [&](int val) {
            res += val;
        });
        return res;
    };
    cout << "top-2 sum in a[3~7] = " << f2(3, 7, 2) << endl;
}

struct Multiplier {
    std::vector<int64_t> m_data;
    template <typename Iterator>
    void reset(Iterator first, Iterator last) {
        m_data.assign(first, last);
    }
    template <typename InitMapping>
    void resize(uint32_t length, InitMapping mapping) {
        m_data.resize(length);
        for (uint32_t i = 0; i != length; i++) m_data[i] = mapping(i);
    }
    int64_t query(int left, int right) const {
        int64_t res = 1;
        for (int i = left; i <= right; i++) res *= m_data[i];
        return res;
    }
};
void test_k_prod() {
    // 写一个简单的例子，表示如何实现区间 k 大数的乘积，或者其他的累积
    // 只需要实现 reset 和 query 两个方法
    // 注意，如果模板为 WaveLet::Tree，则只需要实现 resize 和 query 两个方法
    int a[] = {40, 90, 20, 30, 50, 70, 80, 10, 60};
    cout << "\narr a:";
    for (int i = 0; i < 9; i++) cout << a[i] << " \n"[i == 8];
    OY::WaveLet::Table<uint32_t, Multiplier> S(a, a + 9);
    // 统计 a[3~7] 最小的两个数的乘积
    auto f1 = [&](int l, int r, int k) {
        int64_t res = 1;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_ksmallest(l, r, k, [&](int64_t val) {
            res *= val;
        });
        return res;
    };
    cout << "bottom-2 prod in a[3~7] = " << f1(3, 7, 2) << endl;

    // 统计 a[3~7] 最大的两个数的乘积
    auto f2 = [&](int l, int r, int k) {
        int64_t res = 1;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_klargest(l, r, k, [&](int64_t val) {
            res *= val;
        });
        return res;
    };
    cout << "top-2 prod in a[3~7] = " << f2(3, 7, 2) << endl;
}

int main() {
    test();
    test_k_sum();
    test_k_prod();
}
```

```
#输出如下
1 5 6 3 8 4 4 2 10 1
A[3~6] No.1 = 3
A[3~6] No.2 = 4
A[3~6] No.3 = 4
A[3~6] No.4 = 8
A[0~6].count(1) = 1
A[0~6].count(4) = 2
A[0~6].count(1~4) = 4
A[3~6] rank of (3) = 0
A[3~6] rank of (4) = 1
A[3~6] rank of (5) = 3
A[3~6] rank of (6) = 3
A[3~6] minimum = 3
A[3~6] maximum = 8
A[3~6].max_bitxor(9) = 13
B[1~6] No.1 = app
B[1~6] No.2 = app
B[1~6] No.3 = app
B[1~6] No.4 = banana
B[1~6] No.5 = banana
B[1~6] No.6 = world
B[1~6].count("app") = 3
B[1~6].count("yy") = 0
B[1~6].count("apple"~"cat") = 2
B[1~6] rank of ("ap")     = 0
B[1~6] rank of ("app")    = 0
B[1~6] rank of ("apple")    = 3
B[1~6] rank of ("banana") = 3
B[1~6] minimum = app
B[1~6] maximum = world

arr a:40 90 20 30 50 70 80 10 60
bottom-2 sum in a[3~7] = 40
top-2 sum in a[3~7] = 150

arr a:40 90 20 30 50 70 80 10 60
bottom-2 prod in a[3~7] = 300
top-2 prod in a[3~7] = 5600

```

