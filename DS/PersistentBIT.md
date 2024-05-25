### 一、模板类别

​	数据结构：可持久化树状数组

​	练习题目：

1. [To the moon](https://acm.hdu.edu.cn/showproblem.php?pid=4348)
2. [P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)


### 二、模板功能

​		本数据结构的功能与可持久化线段树几乎一致，用法上略有区别：可持久化线段树可以把每个版本作为一个独立的对象提取出来；而可持久化树状数组会把每个版本放到模板内部，查询某版本信息时需要带上版本号进行查询。

#### 1.建立可持久化树状数组

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示可持久化树状数组中下标编号的变量类型。

   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `typename SizeType` ，表示区间长度类型。
   
   模板参数 `typename Tag` ，表示哈希表类型的标签。

   模板参数 `bool RangeUpdate`​ ，表示是否要支持区间修改。

   模板参数 `bool Rollbackable` ，表示是否要支持区间回滚。

   构造参数 `size_type length` ，表示可持久化树状数组的覆盖范围为 `[0, length)`。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。默认为 `PerBIT::Ignore` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   当 `mapping` 参数为 `Ignore` 对象时，为 $O(1)$ ；否则为 $O(n)$ 。

3. 备注

   可持久化树状数组为树状数组的升级版，具备树状数组的一切功能；区别就在于可持久化树状数组除了对当前版本进行修改和查询之外，还可以对过往的版本进行查询。
   
   可持久化树状数组构造函数构造的是版本零的信息。之后的新建版本号必须是递增的。
   
   模板参数 `SizeType` 的存在，言外之意就是可持久化树状数组可以维护极长的区间。
   
   模板参数 `Tag` 表示通过区间下标找位置的映射方法。如果区间长度较小，可以使用 `PerBIT::VectorTag` ；当区间长度较长时，可以使用 `PerBIT::HashmapTag` 。
   
   模板参数 `Rollbackable` 表示是否支持区间回滚。例如，在版本零的基础上修改得版本一，在版本一的基础上修改得版本二，在版本二的基础上修改得版本三。现在又想在版本一的基础上修改得版本四，那么必须先将版本三、版本二都回滚掉，直到版本一作为最新版本；然后再在版本一的基础上修改得版本四。
   
   **注意：**
   
   树状数组的可持久化为“半可持久化”，只能在**最新版本**的基础上做修改，而不能在过往的版本里随便挑一个版本作为基础。

#### 2.建立可持久化树状数组

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到可持久化树状数组里。



#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示可持久化树状数组要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

   当 `mapping` 参数为 `Ignore` 对象时，为 $O(1)$ ；否则为 $O(n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到可持久化树状数组里。
   
    `mapping` 要求传入一个下标，返回一个 `Tp` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

    本方法要求元素支持 `+=` 运算符。当 `RangeUpdate` 为 `true` 时，要求元素支持 `-` (减法)， `-` (负号)， `+=` 和 `*` 运算符。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到可持久化树状数组里。

   本方法要求元素支持 `+=` 运算符。当 `RangeUpdate` 为 `true` 时，要求元素支持 `-` (减法)， `-` (负号)， `+=` 和 `*` 运算符。

#### 5.预留版本空间(reserve)

1. 数据类型

   输入参数 `size_type max_version` ，表示最大版本号。

2. 时间复杂度

    $O(n)$ 。

#### 6.复制新版本(copy_as)

1. 数据类型

   输入参数 `size_type ver` ，表示新版本的版本号。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   新版本的所有值完全等同于当前的最后一个版本。

   新版本的版本号必须大于当前的版本号。

#### 7.查询版本数量(version_count)

1. 数据类型

   返回类型 `size_type` ，表示当前维护着的版本数量。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   版本数量包括版本零。

   版本数量不包括已经被回滚的版本。

   例如，一开始为版本零，依次新建了版本 `100`， `200` ， `300` ， `400` ， `500` ， `600`，回滚了版本 `500` 和版本 `600` ，再新建版本 `700` 。此时的版本数量为 `6` （分别为 `0` ， `100` ， `200` ， `300` ， `400` ， `700` ）。

#### 8.查询第k个版本的版本号(kth_version)

1. 数据类型

   输入参数 `size_type k` ，表示要查询第 `k` 个版本的版本号。

   返回类型 `size_type` ，表示查询到的版本号。

2. 时间复杂度

    $O(1)$ 。

3. 备注

   当 `k == 0` 时，查询最小的版本号（即版本零）。

   例如，一开始为版本零，依次新建了版本 `100`， `200` ， `300` ， `400` ， `500` ， `600`，回滚了版本 `500` 和版本 `600` ，再新建版本 `700` 。此时的版本数量为 `6` （分别为 `0` ， `100` ， `200` ， `300` ， `400` ， `700` ）。此时 `kth_version(0) == 0` ， `kth_version(1) == 100` ， `kth_version(5) == 700`  。

#### 9.回滚最后一个版本(rollback)

1. 数据类型

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数仅当 `Rollbackable` 为 `true` 时可用。

   请勿在版本数量仅剩一个（也就是版本零）的情况下继续回滚。

#### 10.对最后一个版本单点增值(add)

1. 数据类型

   输入参数 `SizeType i​` ，表示单点增值的下标。

   输入参数 `const Tp &inc​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

   本方法要求元素支持 `+=` 运算符。当 `RangeUpdate` 为 `true` 时，要求元素支持 `-` (负号)， `+=` 和 `*` 运算符。

#### 11.对最后一个版本区间增值(add)

1. 数据类型

   输入参数 `SizeType left` ，表示区间增值的开头下标。

   输入参数 `SizeType right​` ，表示区间增值的结尾下标。(闭区间)

   输入参数 `const Tp &inc` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法仅当 `RangeUpdate` 为 `true` 时合法。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）

   本方法要求元素支持 `-` (负号)， `+=` 和 `*` 运算符。

#### 12.对最后一个版本前缀和查询(presum)

1. 数据类型

   输入参数 `SizeType i`，表示前缀和查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

   本方法要求元素支持 `+=` 运算符。当 `RangeUpdate` 为 `true` 时，要求元素支持 `-` (负号)， `+=` 和 `*` 运算符。

#### 13.对最后一个版本单点查询(query)

1. 数据类型

   输入参数 `SizeType i` ，表示单点查询的下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

   本方法要求元素支持 `-=` 运算符。当 `RangeUpdate` 为 `true` 时，要求元素支持 `+=` 运算符。

#### 14.对最后一个版本区间查询(query)

1. 数据类型

   输入参数 `SizeType left​` ，表示区间查询的开头下标。

   输入参数 `SizeType right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）

   本方法要求元素支持  `-` (减法) 和 `+=` 运算符。当 `RangeUpdate` 为 `true` 时，要求元素支持 `-` (减号)， `+=` 和 `*` 运算符。

#### 15.对最后一个版本查询全部和(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 16.对某个版本查询区间和(query)

1. 数据类型

   输入参数 `size_type ver` ，表示版本号。

   输入参数 `SizeType left​` ，表示区间查询的开头下标。

   输入参数 `SizeType right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log^2 n)$ 。

3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`，且 `left <= right + 1` ）

   本方法要求元素支持  `-` (减法) 和 `+=` 运算符。当 `RangeUpdate` 为 `true` 时，要求元素支持 `-` (减号)， `+=` 和 `*` 运算符。

#### 17.版本间的差分树上求第k个元素(reduce_kth)

1. 数据类型

    输入参数 `size_type base_ver` ，表示基础版本号。

    输入参数 `size_type end_ver` ，表示最终版本号。

    输入参数 `Tp left​` ，表示区间查询的开头下标。

    输入参数 `SizeType right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

    $O(\log^2 n)$ 。

3. 备注

   如果一个版本的树是在另一个版本的基础上经历若干次修改得来，那么两棵树的差就体现了这些修改。如果树为加和树，而要在差分中寻找第 `k` 个元素，可以调用本方法。


### 三、模板示例

```c++
#include "DS/PersistentBIT.h"
#include "IO/FastIO.h"

void test() {
    // 这是一个长度为5的数组
    int A[5] = {10000, 20000, 30000, 40000, 50000};
    // 写一个默认的支持区间加的求和树
    OY::PerBIT::Tree<int64_t, uint32_t, OY::PerBIT::VectorTag, true, true> T(A, A + 5);
    cout << "ver 0:" << T.get_version_view(0) << endl;

    T.copy_as(100);
    cout << "ver 100:" << T.get_version_view(100) << endl;
    // 对最后一个版本进行修改
    T.add(0, 66);
    cout << "ver 100:" << T.get_version_view(100) << endl;

    T.copy_as(200);
    T.add(1, 77);
    T.copy_as(300);
    T.add(2, 88);
    T.copy_as(400);
    T.add(3, 99);
    cout << "ver 400:" << T.get_version_view(400) << endl;

    // 可以回滚一个版本 （把版本 400 彻底删除掉）
    T.rollback();
    T.copy_as(500);
    T.add(3, 4, 101);
    cout << "ver 300:" << T.get_version_view(300) << endl;
    cout << "ver 500:" << T.get_version_view(500) << endl;

    // 在版本之间的差分树上做 kth 查询
    cout << "0-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 0) << endl;
    cout << "1-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 1) << endl;
    cout << "100-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 100) << endl;
    cout << "101-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 101) << endl;
}

int main() {
    test();
}
```

```
#输出如下
ver 0:[10000, 20000, 30000, 40000, 50000]
ver 100:[10000, 20000, 30000, 40000, 50000]
ver 100:[10066, 20000, 30000, 40000, 50000]
ver 400:[10066, 20077, 30088, 40099, 50000]
ver 300:[10066, 20077, 30088, 40000, 50000]
ver 500:[10066, 20077, 30088, 40101, 50101]
0-th elem in ver(300, 500]:3
1-th elem in ver(300, 500]:3
100-th elem in ver(300, 500]:3
101-th elem in ver(300, 500]:4

```
