### 一、模板类别

​	数据结构：`ST` 表

### 二、模板功能

#### 1.建立ST表

1. 数据类型

   模板参数 `typename _Tp` ，表示要进行区间最值查询的元素类型。

   模板参数 `typename _Maximum` ，表示最值函数的类。

   构造参数 `_Iterator __first`​ ，表示区间最值查询的区间头。

   构造参数 `_Iterator __last` ，表示区间最值查询的区间尾。（开区间）

   构造参数 `_Maximum __maxi` ，表示具体的取最值函数。默认为取最大函数。本参数接收类型有：普通函数，函数指针，仿函数，匿名函数，泛型函数。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   `ST`​表处理的问题为区间最值查询，可以是区间最大值、区间最小值，也可以是区间按位或、区间按位与、区间最大公约数。本质上，按位或就是二进制的每位最大值，按位与就是二进制的每位最小值。只要满足**可重复贡献**的性质，就可以使用 `ST` 表。
   
   **可重复贡献**：比如想查询区间 `[1,13]​` 的最大值，可以先查询 `[1,8]` 的最大值，再查询 `[6,13]` 的最大值，再将二者合并。尽管两次查询的区间有重复部分，但是不影响结果。
   
   如果在构造的时候不想直接填区间，可以忽略 `__first` 和 `__last` 两个参数，只填 `__maxi` 参数；之后再通过 `reset​` 填写区间。

#### 2.重置

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间查询的区间头。

   输入参数 `_Iterator __last` ，表示区间查询的区间尾。（开区间）

2. 时间复杂度

   $O(n \cdot \log n)$ 。

3. 备注

   使用迭代器进行重置，可以将区间初状态直接赋到 `ST` 表里。


#### 3.查询

1. 数据类型

   输入参数 `int __left` ，表示要查询的区间的左边界下标。

   输入参数 `int __right` ，表示要查询的区间的右边界下标。（闭区间）

2. 时间复杂度

   $O(1)$ 。
   
3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n-1]`）

### 三、模板示例

```c++
#include "DS/STtable.h"
#include "IO/FastIO.h"

int main() {
    //先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    //建立一个区间最大值 ST 表
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    //一般可以忽略模板参数
    OY::STTable st_max(A, A + 10, mymax);
    cout << "max(A[3~6])     =" << st_max.query(3, 6) << endl;

    //建立一个区间最小值 ST 表
    //甚至可以适用 stl 的最值函数
    OY::STTable st_min(A, A + 10, std::min);
    cout << "min(A[3~6])     =" << st_min.query(3, 6) << endl;

    //建立一个区间最大公约数 ST 表
    //可以在参数框里写 lambda
    OY::STTable st_gcd(A, A + 10, [](int x, int y) { return std::gcd(x, y); });
    cout << "gcd(A[3~6])     =" << st_gcd.query(3, 6) << endl;

    //建立一个区间按位与 ST 表
    //按位与的函数类具有默认构造，可以忽略构造参数
    OY::STTable st_bit_and(A, A + 10, std::bit_and());
    cout << "bit_and(A[3~6]) =" << st_bit_and.query(3, 6) << endl;

    //建立一个区间按位或 ST 表
    //一开始可以是空的，但是注意这种情况下只能使用花括号进行初始化
    OY::STTable st_bit_or(std::bit_or<int>{});
    st_bit_or.reset(A, A + 10);
    cout << "bit_or(A[3~6])  =" << st_bit_or.query(3, 6) << endl;

    //最后的便利化措施：由于实际使用的时候，往往是最大值较多，所以无参构造为最大值 ST 表
    OY::STTable st_default;
    st_default.reset(A, A + 10);
    cout << "max(A[0~9])     =" << st_default.query(0, 9) << endl;
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
max(A[0~9])     =15
```

