### 一、模板类别

​	数据结构：树状数组，树状数组加强版

### 二、模板功能

#### 1.建立树状数组

1. 数据类型

   模板参数 `typename _Tp` ，表示元素类型。

   模板参数 `typename _Plus`​ ，表示加和函数的类，默认为 `std::plus<_Tp>` ，也就是加法。

   模板参数 `typename _Minus` ，表示做差函数的类，默认为 `std::minus<_Tp>` ，也就是减法。

   构造参数 `int __n` ，表示树状数组的覆盖范围为 `[0, n)`。

   构造参数 `_Plus __plus`​ ，表示具体的加和函数，默认值为 `_Plus` 类的默认实例。

   构造参数 `_Minus __minus`​ ，表示具体的做差函数，默认值为 `_Minus` 类的默认实例。

   构造参数 `_Tp __defaultValue` ，表示元素默认值，默认值为 `_Tp` 类的默认实例。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   树状数组处理的问题为单点修改，单点/区间和的查询。树状数组加强版可以同时处理单点/区间修改，单点/区间和的查询。当然，树状数组加强版的时空效率都会略逊于普通的树状数组。所以，如果只处理区间和查询而不进行区间修改，最好使用普通的树状数组；如果只处理区间修改和单点查询，最好对原数组的差分数组建立普通树状数组。

   树状数组要求加和函数的运算符满足**交换律**和**结合律**。

   树状数组要求加和函数和做差函数为**逆运算**，即：`plus(a,b)==c <=> minus(c,a)==b`。

   显然，树状数组非常适合重载加法和减法，或者重载乘法和除法。需要注意的是，重载加减法时，元素默认值传递为 `0` 较为合适；重载乘除法时，元素默认值传递为 `1` 较为合适。

   树状数组通过加和函数求解前缀和，再通过前缀和做差求解任意区间和。这就是为什么需要加和函数和做差函数两个函数。在一些情况下，也可以违反上述规则，比如将加和函数设为`std::max<_Tp>`。那么，显然找不到对应的做差函数，所以只能求解前缀最大值，不能求解任意区间最大值。

   树状数组加强版通过一些差分运算，实现了区间修改。对于加强版，因为应用范围较小，所以没有给出加和函数、做差函数、元素默认值的设置接口，默认为加法、减法、`0`。所以在构造函数中只需要传递区间大小这一个参数。

#### 2.建立树状数组

1. 数据类型

   构造参数 `_Iterator __first` ，表示区间维护的区间头。

   构造参数 `_Iterator __last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到树状数组里。


#### 3.重置

1. 数据类型

   输入参数 `int __n` ，表示树状数组要处理的区间大小。

2. 时间复杂度

   $O(n)$ 。

3. 备注

   调用本函数会将树状数组大小改变，并将之前的合并信息重置。

#### 4.重置

1. 数据类型

   输入参数 `_Iterator __first` ，表示区间维护的区间头。

   输入参数 `_Iterator __last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到树状数组里。

#### 5.单点增值

1. 数据类型

   输入参数 `int i​` ，表示单点增值的下标。

   输入参数 `_Tp __inc​` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 6.区间增值

1. 数据类型

   输入参数 `int __left` ，表示区间增值的开头下标。

   输入参数 `int __right​` ，表示区间增值的结尾下标。(闭区间)

   输入参数 `_Tp __inc` ，表示增量大小。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法仅在树状数组加强版里存在。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 7.前缀和查询

1. 数据类型

   输入参数 `int i`，表示前缀和查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 8.单点查询

1. 数据类型

   输入参数 `int i` ，表示单点查询的下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 9.区间查询

1. 数据类型

   输入参数 `int __left​` ，表示区间查询的开头下标。

   输入参数 `int __right​`，表示区间查询的结尾下标。(闭区间)

2. 时间复杂度

   $O(\log n)$ 。
   
3. 备注

   本方法只有在做差函数合理的时候有意义。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，n)`）

#### 10.查询全部

1. 数据类型

2. 时间复杂度

   $O(\log n)$ 。

#### 11.查询第 $k$ 个元素

1. 数据类型

   输入参数 `_Tp __k​` ，表示要查询的元素从小到大的顺次。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   仅当树状数组的元素类型 `_Tp` 为数字，且加和函数为加法的时候，本方法才有意义。

   顺次 `k​` 表示第 `k+1` 小，顺次 `0` 表示查询最小的元素。

   本函数没有进行参数检查，所以请自己确保 `__k` 合法。（位于`[0，queryAll())`）
   

### 三、模板示例

```c++
#include "DS/BIT.h"
#include "IO/FastIO.h"

int main() {
    //先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 1, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    //不要求极限效率的情况下，树状数组加强版可以解决一切问题
    OY::BIT_ex T(A, A + 10);

    //查询全部的和值
    cout << "sum(A[~])     =" << T.queryAll() << endl;

    //查询下标 2 的值
    cout << "sum(A[2])     =" << T.query(2) << endl;
    //进行单点增值
    T.add(2, 5);
    //查询下标 2 的值
    cout << "sum(A[2])     =" << T.query(2) << endl;

    //将整个区间的值减一
    T.add(0, 9, -1);
    //查询全部的和值
    cout << "sum(A[~])     =" << T.queryAll() << endl;

    //查询下标 3 处的前缀和
    cout << "sum(A[~3])    =" << T.presum(3) << endl;

    //查询区间 [1,3] 的区间和
    cout << "sum(A[1~3])   =" << T.query(1, 3) << endl;

    //查询排名第 28 的元素是谁
    cout << "A.kth(28)     =" << T.kth(28) << endl;

    //*************************************************************************
    //对于树状数组的使用，不追求极限的话，这就够了
    //但是，如果不同时进行区间修改和区间查询，普通的树状数组效率可以跑得比默认树要高
    //方法如下:

    //如果只进行区间查询
    OY::BIT query_tree(A,A+10);
    //该树没有区间修改的接口
    //查询区间 [1,3] 的区间和，为 6
    cout<<query_tree.query(1,3)<<endl;

    //如果只进行区间更新
    //先对原数组求差分，再基于差分数组建树
    std::adjacent_difference(A,A+10,A);
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    OY::BIT modify_tree(A,A+10);
    //先看一下每个值
    for (int i = 0; i < 10; i++)
        cout << modify_tree.query(i) << (i == 9 ? '\n' : ' ');
    //但是，这样输出的值根本不是原数组的值，而是差分值，怎么看原数组值呢？
    //很简单，差分数组的前缀和就是原数组，为
    for (int i = 0; i < 10; i++)
        cout << modify_tree.presum(i) << (i == 9 ? '\n' : ' ');
    //实现 [l,r] 区间增值更新 inc 的方法为：在差分数组上，l 下标处加 inc, r-1 下标处减 inc
    //比如，我们想让区间 [1,3] 增加 10
    modify_tree.add(1,10);
    modify_tree.add(4,-10);
    //看一下差分数组
    for (int i = 0; i < 10; i++)
        cout << modify_tree.query(i) << (i == 9 ? '\n' : ' ');
    //原数组
    for (int i = 0; i < 10; i++)
        cout << modify_tree.presum(i) << (i == 9 ? '\n' : ' ');

    //*************************************************************************
    //再看一些非典型用法：重载乘除法，重载最大值函数
    //先给出一个长度为 10 的数组
    int B[10] = {3, 5, 9, 2, 8, 4, 6, 5, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << B[i] << (i == 9 ? '\n' : ' ');

    //这棵树实现了区间积
    OY::BIT multiply_tree(B,B+10,std::multiplies(),std::divides(),1);
    cout << "product(B[4~6])=" << multiply_tree.query(4,6) << endl;

    //这棵树实现了前缀最大
    auto getmax=[](auto x,auto y){return x>y?x:y;};
    OY::BIT premax_tree(B,B+10,getmax);
    cout << "max(B[~6])     =" << premax_tree.presum(6) << endl;
}
```

```
#输出如下
11 5 9 1 8 4 6 15 7 7
sum(A[~])     =115
sum(A[2])     =9
sum(A[2])     =14
sum(A[~])     =110
sum(A[~3])    =27
sum(A[1~3])   =17
A.kth(28)     =4
15
11 -6 4 -8 7 -4 2 9 -8 0
11 -6 4 -8 7 -4 2 9 -8 0
11 5 9 1 8 4 6 15 7 7
11 4 4 -8 -3 -4 2 9 -8 0
11 15 19 11 8 4 6 15 7 7
3 5 9 2 8 4 6 5 7 7
product(B[4~6])=192
max(B[~6])     =9

```

