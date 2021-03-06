### 一、模板类别

​	数据结构：二维线段树（树状数组套动态开点线段树）。为了和普通二维线段树（动态开点线段树套动态开点线段树）做区分，以下简称 `SegBIT` 。

### 二、模板功能

 `SegBIT` 的功能和普通二维线段树的功能、接口有较多差异。

#### 1.建立 SegBIT

1. 数据类型

   模板参数 `typename _Tp` ，表示元素类型。

   模板参数 `typename _Plus`​ ，表示加和函数的类，默认为 `std::plus<_Tp>` ，也就是加法。

   模板参数 `typename _Minus` ，表示做差函数的类，默认为 `std::minus<_Tp>` ，也就是减法。

   构造参数 `int __row` ，表示 `SegBIT` 的行范围为 `[0, __row)`。

   构造参数 `int __column` ，表示 `SegBIT` 的列范围为 `[0, __column)`。

   构造参数 `_Plus __plus`​ ，表示具体的加和函数，默认值为 `_Plus` 类的默认实例。

   构造参数 `_Minus __minus`​ ，表示具体的做差函数，默认值为 `_Minus` 类的默认实例。

   构造参数 `_Tp __defaultValue` ，表示元素默认值，默认值为 `_Tp` 类的默认实例。

2. 时间复杂度

   $O(1)$ 。

3. 备注

     `SegBIT` 和普通二维线段树相比，在空间上紧凑很多，时间效率高很多。在使用加减法求解求和类问题时，可以认为是改良版的二维线段树。
        
    但是，由于外层是树状数组，所以必须利用差分来进行任意区域的查询。在面对最值查询等问题时，由于不能像加减法一样找到合适的逆运算，所以无法利用差分进行任意区域的查询。
    
     `SegBIT` 要求加和函数的运算符满足**交换律**和**结合律**。
    
     `SegBIT` 要求加和函数和做差函数为**逆运算**，即：`plus(a,b)==c <=> minus(c,a)==b`。
    
    显然， `SegBIT` 非常适合重载加法和减法，或者重载乘法和除法。需要注意的是，重载加减法时，元素默认值传递为 `0` 较为合适；重载乘除法时，元素默认值传递为 `1` 较为合适。
    
    **注意：**在一些博客里，把本数据结构称为“树状数组套主席树”，这是以讹传讹。内层的线段树是普通的线段树，要说特殊点也就是动态开点而已，根本没有进行版本记录。

#### 2.建立 SegBIT 

1. 数据类型

   构造参数 `Ref __ref` ，表示一个寻址函数，通过调用 `__ref(i,j)` 找到原矩阵的 `i` 行 `j` 列的元素。推荐使用匿名函数来传参。

   其它同上。

2. 时间复杂度

   $O(m\cdot n)$ 。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区域初状态直接赋到 `SegBIT` 里。
   
   **注意：**寻址函数的类型和区域操作函数恰好类似，均为二元函数。为了避免混淆，所以在传寻址函数的时候，需要把寻址函数放在 `__row` 和 `__column` 的前面。
   
   **注意：**使用本方法进行初始化，则要求二维区域不能太大。


####  3.重置

1. 数据类型

   输入参数 `int __row` ，表示 `SegZkw` 线段树的行范围为 `[0, __row)`。

   输入参数 `int __column` ，表示 `SegZkw` 线段树的列范围为 `[0, __column)`。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   调用本函数会将 `SegZkw` 线段树大小改变，并将之前的合并信息重置。

#### 4.重置

1. 数据类型

   输入参数 `Ref __ref` ，表示一个寻址函数，通过调用 `__ref(i,j)` 找到原矩阵的 `i` 行 `j` 列的元素。推荐使用匿名函数来传参。

   其它同上。

2. 时间复杂度

   $O(m\cdot n)$ 。

3. 备注

   同上。

   使用迭代器进行重置，可以将区域初状态直接赋到 `SegBIT` 里。

   **注意：**寻址函数要放在 `__row` 和 `__column` 的前面。

   **注意：**使用本方法进行初始化，则要求二维区域不能太大。

#### 5.单点增值

1. 数据类型

   输入参数 `int __row` ，表示单点增值的行下标。

   输入参数 `int __column` ，表示单点增值的列下标。

   输入参数 `_Tp __inc​` ，表示增量大小。

2. 时间复杂度

   $O(\log (m\cdot n))$ 。

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 6.单点查询

1. 数据类型

   输入参数 `int __row` ，表示单点查询的行下标。

   输入参数 `int __column` ，表示单点查询的列下标。

2. 时间复杂度

   $O(\log(m\cdot n))$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 7.区域查询

1. 数据类型

   输入参数 `int __row1` ，表示区域查询的起始行下标。

   输入参数 `int __row2` ，表示区域查询的结束行下标。（闭区间）

   输入参数 `int __column1​` ，表示区域查询的起始列下标。

   输入参数 `int __column2` ，表示区域查询的结束列下标。（闭区间）

2. 时间复杂度

   $O(\log(m\cdot n))$ 。
   
3. 备注

   本函数没有进行参数检查，所以请自己确保下标合法。（位于`[0，m), [0, n)`）

#### 8.查询全部

1. 数据类型

2. 时间复杂度

   $O(\log m)$ 。
   
3. 备注

   本函数与其他函数的时间复杂度不同。

#### 9.按行查询第 $k$ 个元素

1. 数据类型

   输入参数 `int __row1` ，表示区域查询的起始行下标。

   输入参数 `int __row2` ，表示区域查询的结束行下标。（闭区间）

   输入参数 `_Tp __k` ，表示要查询的元素从小到大的顺次。

2. 时间复杂度

   $O(\log (m\cdot n))$ 。

3. 备注

   仅当线段树的元素类型 `_Tp` 为数字，且区间操作函数为加法的时候，本方法才有意义。

   顺次 `k​` 表示第 `k+1` 小，顺次 `0` 表示查询最小的元素。

   本函数没有进行参数检查，所以请自己确保 `__k` 合法。（位于`[0，query(__row1, __row2, 0, m_column-1))`）

   **注意：**这里的排名意义很难理解，可以参考模板示例。

### 三、模板示例

```c++
#include "DS/SegBIT.h"
#include "IO/LeetcodeIO.h"

int main() {
    //二维线段树与一维线段树的用法基本类似，所以只展示一下初始化的特殊方法
    int matrix[4][5] = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
    };
    //除了行数、列数，还需要传递一个寻址函数
    OY::SegBIT T([&](int i, int j) { return matrix[i][j]; }, 4, 5);
    //输出[0,2]行，[1,4]列的和
    cout << "sum(matrix[0~2][1~4])=" << T.query(0, 2, 1, 4) << endl;
    //输出 2 行，排名为 0~8 的数
    for(int k=0;k<=8;k++)
        cout<<"row 2, No."<<k<<" is "<<T.rowKth(2,2,k)<<endl;
    //输出 [1,3] 行，排名为 0 的数
    cout<<"row [1,3], No.0 is "<<T.rowKth(1,3,0)<<endl;
    //输出 [1,3] 行，排名为 10 的数
    cout<<"row [1,3], No.10 is "<<T.rowKth(1,3,10)<<endl;
    //输出 [1,3] 行，排名为 19 的数
    cout<<"row [1,3], No.19 is "<<T.rowKth(1,3,19)<<endl;
}
```

```
#输出如下
sum(matrix[0~2][1~4])=27
row 2, No.0 is 0
row 2, No.1 is 1
row 2, No.2 is 1
row 2, No.3 is 3
row 2, No.4 is 4
row 2, No.5 is 4
row 2, No.6 is 4
row 2, No.7 is 4
row 2, No.8 is 4
row [1,3], No.0 is 0
row [1,3], No.10 is 1
row [1,3], No.19 is 2

```

