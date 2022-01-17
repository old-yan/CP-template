### 一、模板类别

​	数据结构：线段树

### 二、模板功能

#### 1.建立线段树

1. 数据类型

   模板参数 `typename _Tp` ，表示元素类型。

   模板参数 `typename _Operation`  ，表示区间操作函数的类，默认为 `std::plus<_Tp>` ，也就是加法。

   构造参数 `int __n` ，表示线段树的覆盖范围为 `[0, n)`。

   构造参数 `_Operation __op` ，表示具体的区间操作函数。默认为 `_Operation` 类的默认实例。本参数接收类型有：普通函数，函数指针，仿函数，匿名函数，泛型函数。

   构造参数 `_Tp __defaultValue` ，表示元素默认值，默认值为 `_Tp` 类的默认实例。

   构造参数 `_Tp __initValue` ，表示树中元素初始值，默认值为 `_Tp` 类的默认实例。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   线段树处理的问题为单点修改，单点/区间的查询。

   线段树要求区间操作函数的运算符满足**结合律**。

   线段树常常重载加法，维护区间和；或者重载最大值函数，维护区间最值。

   不同于树状数组，线段树对任意区间的查询，都是通过自下而上的累积而来，而非通过差分求得。所以线段树只需要重载一个区间操作函数即可。
   
   **注意：** `__defaultValue` 需要满足：`__op(__defaultValue, __defaultValue)==__defaultValue` 。由于 `_Tp` 类型不一定支持相等运算符，所以 `_check` 函数默认处于注释掉的状态。如果取消注释，则会在建树时进行检查。

#### 2.建立线段树

1. 数据类型

   构造参数 `_Iterator __first` ，表示区间维护的区间头。

   构造参数 `_Iterator __last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到线段树里。
   


#### 3.重置

1. 数据类型

   输入参数 `int __n` ，表示线段树要处理的区间大小。

   输入参数 `_Tp __initValue` ，表示线段树中每个叶结点的初始值，默认为 `_Tp` 类的默认实例。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   调用本函数会将线段树大小改变，并将之前的合并信息重置。

#### 4.重置

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间维护的区间头。

   输入参数 `_Iterator __last` ，表示区间维护的区间尾。（开区间）

   输入参数 `_Tp __initValue` ，表示线段树中叶结点的初始值，默认为 `_Tp` 类的默认实例。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到线段树里。
   
   之所以提供了区间之外还需要 `__initValue` ，是因为线段树要把区间长度凑成二的幂，所以存在一部份区域不能被 `[__first, __last)` 覆盖。

#### 5.单点赋值

1. 数据类型

   输入参数 `int __i​` ，表示单点赋值的下标。

   输入参数 `_Tp __val​` ，表示赋的值。

2. 时间复杂度

   $O(\log n)$ 。

#### 6.单点增值

1. 数据类型

   输入参数 `int __i​` ，表示单点增值的下标。

   输入参数 `_Tp __inc​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

#### 7.单点查询

1. 数据类型

   输入参数 `int __i` ，表示查询的下标。

2. 时间复杂度

   $O(\log n)$ 。


#### 8.区间查询

1. 数据类型

   输入参数 `int __left​` ，表示区间查询的开头下标。

   输入参数 `int __right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。
   

#### 9.查询全部

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 10.查询第 $k$ 个元素

1. 数据类型

   输入参数 `_Tp __k​` ，表示要查询的元素从小到大的顺次。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   仅当线段树的元素类型 `_Tp` 为数字，且区间操作函数为加法的时候，本方法才有意义。

   顺次 `k​` 表示第 `k+1` 小，顺次 `0` 表示查询最小的元素。
   

### 三、模板示例

```c++
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

int main() {
    //线段树可以实现 st 表的所有统计功能，但是查询速度稍微慢一些
    //考虑到线段树是常用数据结构，所以会尽量给出各种使用示范

    //先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    //建立一个区间最大值线段树
    auto mymax = [](int x, int y) {
        return x > y ? x : y;
    };
    //典型用法：用 lambda 传递区间操作函数
    OY::ZkwTree tree_max(A, A + 10, mymax);
    cout << "max(A[3~6])     =" << tree_max.query(3, 6) << endl;

    //建立一个区间最小值线段树
    //可以适用 stl 的最值函数
    OY::ZkwTree tree_min(A, A + 10, std::min);
    cout << "min(A[3~6])     =" << tree_min.query(3, 6) << endl;

    //建立一个区间最大公约数线段树
    //可以在参数框里写 lambda
    OY::ZkwTree tree_gcd(A, A + 10, [](int x, int y) { return std::gcd(x, y); });
    cout << "gcd(A[3~6])     =" << tree_gcd.query(3, 6) << endl;

    //建立一个区间位操作线段树
    //按位与的函数类具有默认构造，可以忽略构造参数
    OY::ZkwTree tree_bit_and(A, A + 10, std::bit_and()); //按位与
    cout << "bit_and(A[3~6]) =" << tree_bit_and.query(3, 6) << endl;

    OY::ZkwTree tree_bit_or(A, A + 10, std::bit_or()); //按位或
    cout << "bit_or(A[3~6])  =" << tree_bit_or.query(3, 6) << endl;

    OY::ZkwTree tree_bit_xor(A, A + 10, std::bit_xor());
    cout << "bit_xor(A[3~6]) =" << tree_bit_xor.query(3, 6) << endl;

    //建立一个区间乘 ST 表
    OY::ZkwTree<int64_t, std::multiplies<int64_t>> tree_mul;
    tree_mul.reset(A, A + 10);
    cout << "prod(A[3~6])    =" << tree_mul.query(3, 6) << endl;

    //便利化措施：由于实际使用的时候，往往是求和树较多，所以无参构造为求和树
    OY::ZkwTree tree_default;
    tree_default.reset(A, A + 10);
    cout << "sum(A[0~9])     =" << tree_default.query(0, 9) << endl;
    cout << "A[4]            =" << tree_default.query(4) << endl;
    tree_default.update(4,18);
    cout << "sum(A[0~9])     =" << tree_default.query(0, 9) << endl;
    tree_default.add(4,100);
    cout << "sum(A[0~9])     =" << tree_default.query(0, 9) << endl;
    //查询排名第 0 的元素是谁
    cout << "A.kth(0)        =" << tree_default.kth(0) << endl;
    //查询排名第 15 的元素是谁
    cout << "A.kth(15)       =" << tree_default.kth(15) << endl;
    //查询排名第 16 的元素是谁
    cout << "A.kth(16)       =" << tree_default.kth(16) << endl;
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
bit_xor(A[3~6]) =6
prod(A[3~6])    =2304
sum(A[0~9])     =84
A[4]            =8
sum(A[0~9])     =94
sum(A[0~9])     =194
A.kth(0)        =0
A.kth(15)       =1
A.kth(16)       =2

```

