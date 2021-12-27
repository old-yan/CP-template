### 一、模板类别

​	数据结构：猫树

### 二、模板功能

#### 1.建立猫树

1. 数据类型

   模板参数 `typename _Tp` ，表示要进行区间查询的元素类型。

   模板参数 `typename _Maximum` ，表示区间操作函数的类。

   构造参数 `_Iterator __first`​ ，表示区间查询的区间头。

   构造参数 `_Iterator __last` ，表示区间查询的区间尾。（开区间）

   构造参数 `_Operation __op` ，表示具体的区间操作函数。默认为取最大函数。本参数接收类型有：普通函数，函数指针，仿函数，匿名函数，泛型函数。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   猫树处理的问题为区间查询，可以是区间最大值、区间最小值，也可以是区间按位或、区间按位与、区间最大公约数。这些是和 `ST` 表统一的功能。除此之外，猫树还可以重载加法、乘法、按位异或，这些是 `ST`表做不到的。

   如果在构造的时候不想直接填区间，可以忽略 `__first` 和 `__last` 两个参数，只填 `__op` 参数；之后再通过 `reset​` 填写区间。

#### 2.重置

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间查询的区间头。

   输入参数 `_Iterator __last` ，表示区间查询的区间尾。（开区间）

2. 时间复杂度

   $O(n \cdot \log n)$ 。

3. 备注

   使用迭代器进行重置，可以将区间初状态直接赋到猫树里。

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
#include "IO/FastIO.h"
#include "DS/catTree.h"

int main() {
    //猫树支持 ST 表的所有功能，而且还支持区间和、区间异或等操作

    //先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    //建立一个区间最大值猫树
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    //猫树和 ST 表一样可以忽略模板参数，
    OY::catTree ct_max(A, A + 10, mymax);
    cout << "max(A[3~6])     =" << ct_max.query(3, 6) << endl;

    //一样的就不多展示了，看看不一样的
    //猫树支持区间异或
    OY::catTree ct_bit_xor(A, A + 10, std::bit_xor());
    cout << "bit_xor(A[3~6]) =" << ct_bit_xor.query(3, 6) << endl;

    //猫树支持区间和
    OY::catTree ct_add(A, A + 10, std::plus());
    cout << "sum(A[3~6])     =" << ct_add.query(3, 6) << endl;

    //猫树支持区间乘
    OY::catTree ct_mul(A, A + 10, std::multiplies());
    cout << "product(A[3~6]) =" << ct_mul.query(3, 6) << endl;
}
```

```
#输出如下
11 5 9 12 8 4 6 15 7 7
max(A[3~6])     =12
bit_xor(A[3~6]) =6
sum(A[3~6])     =30
product(A[3~6]) =2304

```

