### 一、模板类别

​	数据结构：区间 `mex` 查询表

​	练习题目：

1. [2003. 每棵子树内缺失的最小基因值](https://leetcode.cn/problems/smallest-missing-genetic-value-in-each-subtree/)
2. [P4137 Rmq Problem / mex](https://www.luogu.com.cn/problem/P4137)


### 二、模板功能

#### 1.建立表

1. 数据类型


   模板参数 `template<typename>typename BufferType` ，表示内部可持久化压缩线段树的缓存类型。

   构造参数 `size_type length` ，表示区间长度。
   
   构造参数 `InitMapping mapping` ，表示从下标到区间内元素的映射函数。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

#### 2.建立表

1. 数据类型

   构造参数 `Iterator first` ，表示区间维护的区间头。

   构造参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

   其它同上。

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行初始化，可以将区间初状态直接赋到表里。

#### 3.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示区间长度。
   
   输入参数 `InitMapping mapping` ，表示从下标到区间内元素的映射函数。

2. 时间复杂度

   同上。

3. 备注

   同上。

#### 4.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

   同上。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到表里。

#### 5.查询mex(query)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的区间的左端点。
   
   输入参数 `size_type right` ，表示要查询的区间的右端点。（闭区间）
   
   返回类型 `size_type` ，表示查询结果。

2. 时间复杂度

   $O(\log n)$ 。

#### 三、模板示例

```c++
#include "DS/RangeMex.h"
#include "IO/FastIO.h"

void test() {
    // 本模板功能很简单，不多介绍
    int arr[7] = {1, 3, 0, 2, 0, 1, 5};
    OY::RangeMex<> rm(arr, arr + 7);

    cout << "mex of arr[0~1] = " << rm.query(0, 1) << endl;
    cout << "mex of arr[1~3] = " << rm.query(1, 3) << endl;
    cout << "mex of arr[4~6] = " << rm.query(4, 6) << endl;
    cout << "mex of arr[1~6] = " << rm.query(1, 6) << endl;
}

int main() {
    test();
}
```

```
#输出如下
mex of arr[0~1] = 0
mex of arr[1~3] = 1
mex of arr[4~6] = 2
mex of arr[1~6] = 4

```

