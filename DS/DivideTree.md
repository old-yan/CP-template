### 一、模板类别

​	数据结构：划分树。

​	练习题目：

1. [P1972 [SDOI2009] HH的项链](https://www.luogu.com.cn/problem/P1972)
2. [P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)

### 二、模板功能

#### 1.建立划分树

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示树中下标、区间下标的变量类型。

   模板参数 `typename Node` ，表示结点类型。

   模板参数 `size_type MAX_NODE` ，表示最大结点数，默认为 `1<<22` 。

   构造参数 `size_type length` ，表示划分树的覆盖范围为 `[0, length)`。。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   划分树处理的问题为区间第 `k​` 序值查询，要求区间内的元素能够按照指定排序函数进行排序。

   不同于之前版本的划分树设计，目前的划分树的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让划分树工作。

   对于划分树来说，结点须满足以下要求：

   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `comp` ，接受两个 `value_type` 参数，返回前者小于后者的布尔值
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值。

   至此，划分树所需的结点功能已经足够。

   **注意：**

   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。

   划分树只能处理静态区间上的问题，所以没有提供修改区间的入口。

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

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到划分树里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到划分树里。

#### 5.区间第k序值查询(quantile)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `size_type k` ，表示要查找的元素的大小次序。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    `k` 的取值范围从 `0` 开始，表示**如果**将原区间的 `[l, r]` 内按照指定排序函数进行排序，那么排在 `[l + k]` 位置的元素

   本函数没有进行参数检查，所以请自己确保下标合法，且 `k​` 处于 `[0,r-l]` 范围内。


#### 6.区间某数频率查询(count)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const value_type &val` ，表示要查找的元素。

   返回类型 `size_type` ，表示查询到的频率。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 7.区间某范围内数字的频率查询(count)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const value_type &minimum` ，表示要求范围的最小值。

   输入参数 `const value_type &maximum` ，表示要求范围的最大值。(闭区间)

   返回类型 `size_type` ，表示查询到的频率。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 8.区间某数排名查询(rank)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   输入参数 `const value_type &val` ，表示要查找的元素。

   返回类型 `size_type` ，表示查询到的比其更小的元素的数目。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数等价于区间内小于 `val` 的元素数目。

   本函数没有进行参数检查，所以请自己确保下标合法。

#### 9.区间最小值查询(minimum)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `Tp` ，表示查询到的元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

    本函数没有进行参数检查，所以请自己确保下标合法。

#### 9.区间最大值查询(maximum)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

   返回类型 `value_type` ，表示查询到的元素。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。
   
### 三、模板示例

```c++
#include "DS/DivideTree.h"
#include "IO/FastIO.h"

int main() {
    // 先给出一个长度为 10 的区间
    int A[10] = {1, 5, 6, 3, 8, 4, 4, 2, 10, 1};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个默认划分树（排序函数为小于号）
    // 自定义排序函数：按长度排序
    // 注意 C++14 之前必须显式声明比较函数的类
    auto dt = OY::make_DivideTree<std::less<int>, 1000>(A, A + 10);

    cout << "A[3~6] No.1 = " << dt.quantile(3, 6, 0) << endl;
    cout << "A[3~6] No.2 = " << dt.quantile(3, 6, 1) << endl;
    cout << "A[3~6] No.3 = " << dt.quantile(3, 6, 2) << endl;
    cout << "A[3~6] No.4 = " << dt.quantile(3, 6, 3) << endl;

    // 再应用到字符串数组上
    std::vector<std::string> B{"hello", "world", "I", "am", "a", "robot"};
    for (int i = 0; i < B.size(); i++) cout << B[i] << (i == B.size() - 1 ? '\n' : ' ');

        // 自定义排序函数：按长度排序
        // 注意 lambda 语法仅在 C++20 后支持
#if CPP_STANDARD >= 202002L
    auto comp = [](auto &x, auto &y) {
        return x.size() < y.size();
    };
#else
    struct {
        bool operator()(const std::string &x, const std::string &y) const {
            return x.size() < y.size();
        };
    } comp;
#endif
    auto dt_str = OY::make_DivideTree<decltype(comp), 1000>(B.begin(), B.end());

    cout << "B[1~4] No.1 = " << dt_str.quantile(1, 4, 0) << endl;
    cout << "B[1~4] No.2 = " << dt_str.quantile(1, 4, 1) << endl;
    cout << "B[1~4] No.3 = " << dt_str.quantile(1, 4, 2) << endl;
    cout << "B[1~4] No.4 = " << dt_str.quantile(1, 4, 3) << endl;
    // 可以看出，划分树可以保留相等元素之间的相对顺序
}
```

```
#输出如下
1 5 6 3 8 4 4 2 10 1
A[3~6] No.1 = 3
A[3~6] No.2 = 4
A[3~6] No.3 = 4
A[3~6] No.4 = 8
hello world I am a robot
B[1~4] No.1 = I
B[1~4] No.2 = a
B[1~4] No.3 = am
B[1~4] No.4 = world

```

