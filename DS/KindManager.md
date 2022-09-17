### 一、模板类别

​	数据结构：区间种类数管理。

### 二、模板功能


#### 1.构造

1. 数据类型

   模板参数 `typename _Tp` ，表示区间元素的类型。

   模板参数 `typename _Map` ，表示本数据结构使用的哈希表。哈希表将用于记录某元素的位置，默认为 `std::map<_Tp,uint32_t>` 。

   模板参数 `typename _Iterator` ，表示区间上的迭代器类型。

   构造参数 `_Iterator __first` ，表示区间开始位置。

   构造参数 `_Iterator __last` ，表示区间结尾位置。（开区间）

2. 时间复杂度

   $O(n\cdot \log n)$ 。

3. 备注

   本数据结构基于 `可持久化动态开点线段树` ，对一个静态区间的种类数进行维护。


#### 2.查询区间中的不同种类数

1. 数据类型

   输入参数 `uint32_t __left` ，表示要查询的区间起始下标。

   输入参数 `uint32_t __right` ，表示要查询的区间结尾下标。

   返回类型 `uint32_t` ，表示该区间中不同元素的种类数。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保参数合法。（ `__left` 和 `__right` 位于 `[0, __n]` ）


### 三、模板示例

```c++
#include "DS/KindManager.h"
#include "IO/FastIO.h"

int main() {
    std::string A[] = {"apple", "banana", "apple", "orange", "banana"};
    //常规的元素类型，可以使用 std::map
    OY::KindManager<std::string> km(A, A + 5);
    cout << "there are " << km.query(0, 3) << " kinds of fruits in [0, 3].\n";

    //如果区间内的元素类型为 int ，且范围较小，可以使用数组作为哈希表，更为高效
    int B[] = {15, 98, 10, 98, 98, 98};
    //常规的元素类型，可以使用 std::map
    OY::KindManager<int, int[99]> km2(B, B + 6);
    cout << "there are " << km2.query(1, 5) << " kinds of integers in [1, 5].\n";
}
```

```
#输出如下
there are 3 kinds of fruits in [0, 3].
there are 2 kinds of integers in [1, 5].

```

