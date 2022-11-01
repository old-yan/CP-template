### 一、模板类别

​	数据结构：`sqrt` 树。

### 二、模板功能

#### 1.建立sqrt树

1. 数据类型

   模板参数 `typename _Tp` ，表示要进行区间维护的元素类型。

   模板参数 `typename _Operation`  ，表示区间操作函数的类，默认为 `std::plus<_Tp>` ，也就是加法。

   构造参数 `int __n`​ ，表示区间维护的长度。

   构造参数 `_Operation __op` ，表示具体的区间操作函数。默认为 `_Operation` 类的默认实例。本参数接收类型有：普通函数，函数指针，仿函数，匿名函数，泛型函数。

   构造参数 `_Tp __defaultValue` ，表示默认元素值。默认为 `_Tp` 类的默认值。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   `Sqrt`树处理的问题为区间维护，可以是区间最大值、区间最小值，也可以是区间按位或、区间按位与、区间最大公约数以及加法、乘法、按位异或等。

   如果在构造的时候不想直接填区间长度，可以设为 `0` 。
   
   `sqrt` 树相对于 `st` 表的优势在于：占用空间小，初始化用时短，单点修改的用时短。
   
   在测试中，`sqrt` 树的查询效率与 `st` 表相当。
   
   `sqrt`树在修改和查找操作中，更擅长查找。但是也支持修改操作。

#### 2.建立sqrt树

1. 数据类型

   构造参数 `_Iterator __first`​ ，表示区间维护的区间头。

   构造参数 `_Iterator __last` ，表示区间维护的区间尾。（开区间）

   其余同上。

2. 时间复杂度

   同上。

3. 备注

   使用迭代器进行初始化，可以将区间初状态直接赋到 `sqrt` 树里。

#### 3.重置

1. 数据类型

   输入参数 `int __n` ，表示重置为的区间长度。

2. 时间复杂度

   $O(n)$ 。

#### 4.重置

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间查询的区间头。

   输入参数 `_Iterator __last` ，表示区间查询的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   使用迭代器进行重置，可以将区间初状态直接赋到 `sqrt` 树里。

#### 5.单点修改

1. 数据类型

   输入参数 `int __i` ，表示要修改的点的下标。

   输入参数 `_Tp __val` ，表示要修改为的值。

2. 时间复杂度

   $O(\sqrt n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.单点查询

1. 数据类型

   输入参数 `int __i` ，表示要查询的点的下标。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.区间查询

1. 数据类型

   输入参数 `int __left` ，表示要查询的区间的左边界下标。

   输入参数 `int __right` ，表示要查询的区间的右边界下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本方法大多数情况下为 $O(1)$ ，少数情况下为 $O(\log n)$ ，均摊下来仍为 $O(1)$ 。
   
   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 8.查询全部

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

### 三、模板示例

```c++
#include "DS/SqrtTree.h"
#include "IO/FastIO.h"

int main() {
    //先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    //建立一个区间最大值 sqrt 树
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    //一般可以忽略模板参数
    OY::SqrtTree sqrt_max(A, A + 10, mymax);
    cout << "max(A[3~6])     =" << sqrt_max.query(3, 6) << endl;

    //建立一个区间最小值 sqrt 树
    //甚至可以适用 stl 的最值函数
    OY::SqrtTree sqrt_min(A, A + 10, std::min, INT_MAX);
    cout << "min(A[3~6])     =" << sqrt_min.query(3, 6) << endl;

    //建立一个区间最大公约数 sqrt 树
    //可以在参数框里写 lambda
    OY::SqrtTree sqrt_gcd(A, A + 10, [](int x, int y) { return std::gcd(x, y); });
    cout << "gcd(A[3~6])     =" << sqrt_gcd.query(3, 6) << endl;

    //建立一个区间按位与 sqrt 树
    //按位与的函数类具有默认构造，可以忽略构造参数
    OY::SqrtTree sqrt_bit_and(A, A + 10, std::bit_and());
    cout << "bit_and(A[3~6]) =" << sqrt_bit_and.query(3, 6) << endl;

    //建立一个区间按位或 sqrt 树
    //一开始可以是空的
    OY::SqrtTree sqrt_bit_or(0, std::bit_or<int>());
    sqrt_bit_or.reset(A, A + 10);
    cout << "bit_or(A[3~6])  =" << sqrt_bit_or.query(3, 6) << endl;

    //最后的便利化措施：由于实际使用的时候，往往是最大值较多，所以无参构造为最大值 sqrt 树
    OY::SqrtTree sqrt_default;
    sqrt_default.reset(A, A + 10);
    cout << "sum(A[0~9])     =" << sqrt_default.query(0, 9) << endl;

    //最新的 sqrt 树支持开指定大小的空表；也支持对单点进行修改（当然速度一般）
    OY::SqrtTree new_sqrt(10,std::min<int>,INT_MAX);
    for(int i=0;i<10;i++)new_sqrt.update(i,A[i]);
    cout << "min(A[3~6]) =" << new_sqrt.query(3, 6) << endl;

    new_sqrt.update(4,2);
    cout << "A[4]        =" << new_sqrt.query(4) << endl;
    cout << "min(A[3~6]) =" << new_sqrt.query(3, 6) << endl;
}
```

```
#输出如下
11 5 9 12 8 4 6 15 7 7
max(A[3~6])     =12
min(A[3~6])     =4
gcd(A[3~6])     =2
bit_and(A[3~6]) =0
bit_or(A[3~6])  =14
sum(A[0~9])     =84
min(A[3~6]) =4
A[4]        =2
min(A[3~6]) =2

```

