### 一、模板类别

​	数据结构：`Merge Sort` 树（国内称划分树）。

​	练习题目：

1. [Minimum Sum](https://acm.hdu.edu.cn/showproblem.php?pid=3473)
2. [P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)
3. [P4094 [HEOI2016/TJOI2016] 字符串](https://www.luogu.com.cn/problem/P4094)
4. [P10814 【模板】离线二维数点](https://www.luogu.com.cn/problem/P10814)
5. [U311262 求区间后继](https://www.luogu.com.cn/problem/U311262)
6. [fsl 的背包](https://ac.nowcoder.com/acm/problem/263978)
7. [Range Kth Smallest](https://judge.yosupo.jp/problem/range_kth_smallest)(https://github.com/yosupo06/library-checker-problems/issues/310)


### 二、模板功能

​		划分树其实是小波树的变种，大部分功能可以对标小波树；在元素范围较大、需要离散化的场景下，性能可能反超小波树。

#### 1.建立划分树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `typename Compare` ，表示元素的比较函数的类型。
   
   模板参数 `typename SumTable` ，表示对元素进行累积的表的类型。默认为 `VoidTable` ，表示不进行累积。

   构造参数 `size_type length` ，表示划分树的覆盖范围为 `[0, length)`。。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。
   
   构造参数 `TableMapping table_mapping` ，表示将元素存储到 `SumTable` 中时对元素进行的映射变换。默认为 `MS::Ignore` 表示不进行变换。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   划分树处理的问题为区间最小值查询、区间最大值查询、区间某数频率查询、区间某值域的数的频率查询、区间第 `k​` 序值查询。

   当 `SumTable` 不为空表类型时，可以进行区间前 `k` 个最小数的累积值的查询、区间前 `k` 个最大数的累积值的查询。具体的累积内容，由表的类型决定。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `Tp` 对象。
   
   划分树只能处理静态区间上的问题，所以没有提供修改区间的入口。
   
   **注意：**
   
   划分树对 `SumTable` 的要求与小波树对 `SumTable` 的要求不同，不需要实现 `resize` 方法，只需要实现 `reset` 方法；除了需要实现两个参数的 `query` 以实现区间查询外，还需要实现一个参数的 `query` 以实现单点查询。
   
   如果直接传递 `OY::AdjDiff` 中的模板，用来查询区间和，可能会出现复杂度超标。因为 `OY::AdjDiff` 中的模板在单点查询时，会转换到值形态。

#### 2.建立划分树

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到划分树里。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示划分树要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

   输入参数 `TableMapping table_mapping` ，表示将元素存储到 `SumTable` 中时对元素进行的映射变换。默认为 `MS::Ignore` 表示不进行变换。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到划分树里。

    `mapping` 要求传入一个下标，返回一个 `Tp` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。
   
    本函数与 `resize_mapping_with_index` 的区别是， `table_mapping` 的入参为元素值；本函数不能区分不同位置的相等元素。

#### 4.重置(resize_mapping_with_index)

1. 数据类型

   输入参数 `TableMapping table_mapping` ，表示将元素存储到 `SumTable` 中时对元素进行的映射变换。

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   本函数与 `resize` 的区别是， `table_mapping` 的入参为元素值和其下标；本函数可以区分不同位置的相等元素。

#### 5.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   输入参数 `TableMapping table_mapping` ，表示将元素存储到 `SumTable` 中时对元素进行的映射变换。默认为 `MS::Ignore` 表示不进行变换。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到划分树里。
   
   本函数与 `reset_mapping_with_index` 的区别是， `table_mapping` 的入参为元素值；本函数不能区分不同位置的相等元素。

#### 6.重置(reset_mapping_with_index)

1. 数据类型

   输入参数 `TableMapping table_mapping` ，表示将元素存储到 `SumTable` 中时对元素进行的映射变换。

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   本函数与 `reset` 的区别是， `table_mapping` 的入参为元素值和其下标；本函数可以区分不同位置的相等元素。

#### 7.区间第k序值查询(quantile)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `size_type k` ，表示要查找的元素的大小次序。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    `k` 的取值范围从 `0` 开始，表示**如果**将原区间的 `[l, r]` 内按照指定排序函数进行排序，那么排在 `[l + k]` 位置的元素

   本函数没有进行参数检查，所以请自己确保下标合法，且 `k​` 处于 `[0,r-l]` 范围内。


#### 8.区间某数频率查询(count)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const Tp &val` ，表示要查找的元素。

   返回类型 `size_type` ，表示查询到的频率。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 9.区间某范围内数字的频率查询(count)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const Tp &minimum` ，表示要求范围的最小值。

   输入参数 `const Tp &maximum` ，表示要求范围的最大值。(闭区间)

   返回类型 `size_type` ，表示查询到的频率。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 10.区间某数排名查询(rank)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const Tp &val` ，表示要查找的元素。

   返回类型 `size_type` ，表示查询到的比其更小的元素的数目。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数等价于区间内小于 `val` 的元素数目。

   本函数没有进行参数检查，所以请自己确保下标合法。

#### 11.区间最小值查询(minimum)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `Tp` ，表示查询到的元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 12.区间最大值查询(maximum)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `Tp` ，表示查询到的元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。
#### 13.对区间排名连续的若干个元素的累计值调用回调(do_for_rank_range)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `size_type rk1` ，表示最小排名。

   输入参数 `size_type rk2` ，表示最大排名。

   输入参数 `Callback &&call` ，表示对查询到的元素们的累计值调用的回调。

2. 时间复杂度

    $O(\log n)$ 。

3. 备注

   `rk1` 和 `rk2` 的取值范围从 `0` 开始，表示**如果**将原区间的 `[l, r]` 内按照指定排序函数进行排序，那么查询的是排在 `[l + rk1 ~ l + rk2]` 位置的元素。

   本函数没有进行参数检查，所以请自己确保下标合法，且 `rk1​` 和 `rk2` 处于 `[0,r-l]` 范围内，且 `rk1 <= rk2` 。

#### 14.对区间值连续的若干个元素的累计值调用回调(do_for_value_range)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const Tp &floor` ，表示最小值。

   输入参数 `const Tp &ceil` ，表示最大值。

   输入参数 `Callback &&call` ，表示对查询到的元素们的累计值调用的回调。

2. 时间复杂度

    $O(\log n)$ 。

### 三、模板示例

```c++
#include "DS/MergeSortTree.h"
#include "IO/FastIO.h"

void test() {
    // 先给出一个长度为 10 的区间
    int A[10] = {1, 5, 6, 3, 8, 4, 4, 2, 10, 1};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个默认归并树
    auto wt = OY::MS::Tree<uint32_t>(A, A + 10);

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

    std::string B[] = {"hello", "app", "app", "world", "banana", "app", "banana", "hello"};
    // 建立一个默认归并树
    auto wt2 = OY::MS::Tree<std::string>(B, B + 8);

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

    using SumTable = OY::AdjSumTable<int, true>;
    OY::MS::Tree<uint32_t, std::less<uint32_t>, SumTable> S(a, a + 9);

    // 统计 a[3~7] 最小的两个数的和
    auto f1 = [&](int l, int r, int k) {
        int res = 0;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, 0, k - 1, [&](int val) {
            res += val;
        });
        return res;
    };
    cout << "bottom-2 sum in a[3~7] = " << f1(3, 7, 2) << endl;

    // 统计 a[3~7] 最大的两个数的和
    auto f2 = [&](int l, int r, int k) {
        int res = 0;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, r - l + 1 - k, r - l, [&](int val) {
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
    int64_t query(int pos) const {
        return m_data[pos];
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
    int a[] = {40, 90, 20, 30, 50, 70, 80, 10, 60};
    cout << "\narr a:";
    for (int i = 0; i < 9; i++) cout << a[i] << " \n"[i == 8];
    OY::MS::Tree<uint32_t, std::less<uint32_t>, Multiplier> S(a, a + 9);
    // 统计 a[3~7] 最小的两个数的乘积
    auto f1 = [&](int l, int r, int k) {
        int64_t res = 1;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, 0, k - 1, [&](int64_t val) {
            res *= val;
        });
        return res;
    };
    cout << "bottom-2 prod in a[3~7] = " << f1(3, 7, 2) << endl;

    // 统计 a[3~7] 最大的两个数的乘积
    auto f2 = [&](int l, int r, int k) {
        int64_t res = 1;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, r - l + 1 - k, r - l, [&](int64_t val) {
            res *= val;
        });
        return res;
    };
    cout << "top-2 prod in a[3~7] = " << f2(3, 7, 2) << endl;
}

void test_value_range_sum() {
    // 借助差分模板，统计区间 k 大数的和
    int a[] = {40, 90, 20, 30, 50, 70, 80, 10, 60};
    cout << "\narr a:";
    for (int i = 0; i < 9; i++) cout << a[i] << " \n"[i == 8];

    using SumTable = OY::AdjSumTable<int, true>;
    OY::MS::Tree<uint32_t, std::less<uint32_t>, SumTable> S(a, a + 9);

    // 统计 a[1~7] 里，值在 [10, 60] 的数的和
    int res = 0;
    // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
    S.do_for_value_range(1, 7, 10, 60, [&](int val) {
        res += val;
    });
    cout << "sum of elem([10, 60]) in a[1~7] = " << res << endl;
}

int main() {
    test();
    test_k_sum();
    test_k_prod();
    test_value_range_sum();
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

arr a:40 90 20 30 50 70 80 10 60
sum of elem([10, 60]) in a[1~7] = 110

```

