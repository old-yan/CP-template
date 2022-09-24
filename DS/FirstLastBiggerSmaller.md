### 一、模板类别

​	数据结构：区间查询较大较小树

### 二、模板功能

#### 1.构建

1. 数据类型

   模板参数 `typename _Tp` ，表示要进行区间查询的元素类型。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本数据结构处理的问题为，给出一个区间，给出一个阈值。问这个区间内第一个/最后一个，大于/小于阈值的元素位置。

#### 2.构建

1. 数据类型

   构造参数 `_Iterator __first`​ ，表示要进行查询的区间首迭代器。

   构造参数 `_Iterator __last`​ ，表示要进行查询的区间尾迭代器。

   其余同上。

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   使用迭代器进行初始化，可以将区间初状态直接赋到数据结构里。

#### 3.查询区间内第一个大于阈值的元素位置

1. 数据类型

   输入参数 `uint32_t __left`​ ，表示要进行查询的区间起始处的下标。

   输入参数 `uint32_t __right`​ ，表示要进行查询的区间结尾处的下标。

   输入参数 `_Tp __item` ，表示查询的阈值。

   返回类型 `uint32_t` ，表示结果下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注
   
   如果没有符合条件的答案，返回 `-1` 。

#### 4.查询区间内最后一个大于阈值的元素位置

1. 数据类型

   输入参数 `uint32_t __left`​ ，表示要进行查询的区间起始处的下标。

   输入参数 `uint32_t __right`​ ，表示要进行查询的区间结尾处的下标。

   输入参数 `_Tp __item` ，表示查询的阈值。

   返回类型 `uint32_t` ，表示结果下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注
   
   如果没有符合条件的答案，返回 `-1` 。

#### 5.查询区间内第一个小于阈值的元素位置

1. 数据类型

   输入参数 `uint32_t __left`​ ，表示要进行查询的区间起始处的下标。

   输入参数 `uint32_t __right`​ ，表示要进行查询的区间结尾处的下标。

   输入参数 `_Tp __item` ，表示查询的阈值。

   返回类型 `uint32_t` ，表示结果下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注
   
   如果没有符合条件的答案，返回 `-1` 。

#### 6.查询区间内最后一个小于阈值的元素位置

1. 数据类型

   输入参数 `uint32_t __left`​ ，表示要进行查询的区间起始处的下标。

   输入参数 `uint32_t __right`​ ，表示要进行查询的区间结尾处的下标。

   输入参数 `_Tp __item` ，表示查询的阈值。

   返回类型 `uint32_t` ，表示结果下标。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注
   
   如果没有符合条件的答案，返回 `-1` 。


### 三、模板示例

```c++
#include "DS/FirstLastBiggerSmaller.h"
#include "IO/FastIO.h"

int main() {
    //先给出一个长度为 10 的数组
    int A[10] = {11, 5, 9, 12, 8, 4, 6, 15, 7, 7};
    for (int i = 0; i < 10; i++)
        cout << A[i] << (i == 9 ? '\n' : ' ');

    OY::FirstLastBiggerSmaller<int> flbs(A, A + 10);

    auto index = flbs.queryFirstBigger(0, 7, 11);
    cout << "Among A[0~7], A[" << index << "] is the first to be bigger than 11\n";

    index = flbs.queryFirstSmaller(0, 7, 9);
    cout << "Among A[0~7], A[" << index << "] is the first to be smaller than 9\n";

    index = flbs.queryLastBigger(8, 9, 7);
    if (index == -1) {
        cout << "Among A[8~9], no elem is bigger than 7\n";
    }
}
```

```
#输出如下
11 5 9 12 8 4 6 15 7 7
Among A[0~7], A[3] is the first to be bigger than 11
Among A[0~7], A[1] is the first to be smaller than 9
Among A[8~9], no elem is bigger than 7

```

