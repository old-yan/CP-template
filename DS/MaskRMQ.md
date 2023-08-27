### 一、模板类别

​	数据结构：状压 `RMQ` 。

### 二、模板功能

​		本数据结构分两个版本，一个版本的查询结果为最值所在的下标，一个版本的查询结果为最值的值。可以根据需要使用。以下文档按照返回下标进行编写。

#### 1.建立状压RMQ

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标、区间下标的变量类型。

   模板参数 `typename Node` ，表示结点类型。

   模板参数 `typename MaskType` ，表示用于状态压缩的类型，必须为 `uint32_t` 或者 `uint64_t` 之一。

   模板参数 `template <typename, size_type> typename InterTable` ，表示用于块间区间维护的模板，推荐使用 `OY::ST::Table` 。

   模板参数 `size_type MAX_NODE` ，表示最大结点数。

   构造参数 `size_type length` ，表示状压 `RMQ` 的覆盖范围为 `[0, length)`。默认值为 `0` 。

   构造参数 `InitMapping mapping` ，表示在初始化时，从下标到值的映射函数。默认为 `MaskRMQ::NoInit` 。接收类型可以为普通函数，函数指针，仿函数，匿名函数，泛型函数等。

2. 时间复杂度

    $O(n)$ ；相当于 `InterTable` 执行一个 $\frac n {\log n}$ 规模的初始化，再 执行 $\frac n {\log n}$ 个 $\log n$ 规模的初始化。

3. 备注

   状压 `RMQ` 处理的问题为区间最值的维护，而且必须是严格意义上的最值，而不能是按位或、按位与等等。这一点是与 `ST` 表不同的。

   一般来说， `InterTable` 设为 `OY::ST::Table` 或者 `OY::Cat::Table` 乃至 `OY::Zkw::Tree` 均可，不过考虑到本数据结构只能维护最值，所以一般设为 `OY::ST::Table` 或者 `OY::Cat::Table` 即可。

   目前的状压 `RMQ` 的大量细节都放到了模板参数 `Node` 中，只需要设计好 `Node` 即可让状压 `RMQ` 工作。
   
   对于状压 `RMQ` 来说，结点须满足以下要求：
   
   1. 声明 `value_type` 为值类型；
   2. 定义静态函数 `comp` ，接受两个 `value_type` 参数，返回前者是否小于后者的布尔值；
   3. 实现成员函数 `set` ，接受一个 `value_type` 参数，将此值赋给本结点；
   4. 实现成员函数 `get` ，返回本结点的值。
   
   至此，状压 `RMQ` 所需的结点功能已经足够。此外，结点仍需要满足 `InterTable` 对结点的要求。
   
   **注意：**
   
   构造参数中的 `mapping` 参数，入参为下标，返回值须为一个 `value_type` 对象。默认情况下， `mapping` 为 `MaskRMQ::NoInit` 类，表示不进行初始化，比如要建立一颗空的最大值表，由于全局变量值本身就是零，所以无需进行初始化。
   
   **注意：**
   
    `InterTable` 参数，必须为模板类，第一个模板参数为结点类型，第二个模板参数为最大结点数。显然， `OY::Accumulator::Table` 并不满足这个性质，多出了两个参数。所以我们另外定义了两个参数的 `PrefixTable` ，以便作为 `InterTable` 使用。
   
   **注意：**
   
   本数据结构的各种操作的时间复杂度，与 `InterTable` 密切相关。姑且按照 `InterTable` 为 `OY::ST::Table` 来填写时间复杂度；同时会注明使用其他参数时的复杂度变化。

#### 2.建立状压RMQ

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到状压 `RMQ` 里。


#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示状压 `RMQ` 要处理的区间大小。

   输入参数 `InitMapping mapping` ，表示初始化时，从下标到值的映射函数。

2. 时间复杂度

    $O(n)$ ；相当于 `InterTable` 执行一个 $\frac n {\log n}$ 规模的初始化，再 执行 $\frac n {\log n}$ 个 $\log n$ 规模的初始化。

3. 备注

   使用映射函数进行初始化，可以将区间初状态直接赋到状压 `RMQ` 里。

    `mapping` 要求传入一个下标，返回一个 `value_type` 类型的值。在调用时，会按照下标从 `0` 到 `length-1` 依次调用。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到状压 `RMQ` 里。


#### 5.单点赋值(modify)

1. 数据类型

   输入参数 `size_type i​` ，表示单点赋值的下标。

   输入参数 `const value_type &val​` ，表示赋的值。

2. 时间复杂度

   $O(\frac n \log n+w)$ ，此处 `w` 指 `MaskType` 位宽度；相当于 `InterTable` 进行一次单点赋值，然后再进行一次 `w` 长度的操作。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.单点查询(query)

1. 数据类型

   输入参数 `size_type i` ，表示查询的下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）


#### 7.区间查询(query)

1. 数据类型

   输入参数 `size_type left​` ，表示区间查询的开头下标。

   输入参数 `size_type right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

    $O(1)$ ；相当于 `InterTable` 进行一次区间查询。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 8.查询全部(query_all)

1. 数据类型

2. 时间复杂度

   $O(1)$ ；相当于 `InterTable` 进行一次区间查询。

#### 9.根据下标获取值(get)

1. 数据类型

   输入参数 `size_type index` ，表示查询的下标。

   返回类型 `value_type` ，表示区间里下标对应的元素值。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法仅存在于查询最值下标的版本中。

### 三、模板示例

```c++
#include "DS/MaskRMQ.h"
#include "IO/FastIO.h"

int main() {
    // 先给出一个长度为 10 的数组
    int A[10] = {100, 99, 100, 101, 102, 103, 102, 103, 102, 103};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个区间最大值、返回最大值下标的状压 RMQ 表
    OY::MaskRMQMaxIndexTable<int> S(A, A + 10);
    cout << "max(A[3~6]).index = " << S.query(3, 6) << endl;

    // 建立一个区间最小值、返回最小值的值的状压 RMQ 表
    OY::MaskRMQMinValueTable<int> S2(A, A + 10);
    cout << "min(A[3~6])       = " << S2.query(3, 6) << endl;
}
```

```
#输出如下
100 99 100 101 102 103 102 103 102 103
max(A[3~6]).index = 5
min(A[3~6])       = 101

```
