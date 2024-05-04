### 一、模板类别

​	数据结构：全局区间元素数量查询表

​	练习题目：

1. [Static Range Frequency](https://judge.yosupo.jp/problem/static_range_frequency)(https://github.com/yosupo06/library-checker-problems/issues/666)


### 二、模板功能

#### 1.构造函数

1. 数据类型

   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `size_type BUFFER` ，表示哈希表大小。

2. 时间复杂度

   当开在全局时， $O(1)$ ；否则为 $O(n)$ 。

3. 备注

   本模板基于哈希表进行区间元素数量的维护；元素类型需要提供哈希函数和想等函数才可以使用本模板。

#### 2.重置(resize)

1. 数据类型

   输入参数 `size_type length` ，表示区间长度。
   
   输入参数 `InitMapping mapping` ，表示从下标到区间内元素的映射函数。

2. 时间复杂度

    $O(n)$ 。

3. 备注

   同上。

#### 3.重置(reset)

1. 数据类型

   输入参数 `Iterator first` ，表示区间维护的区间头。

   输入参数 `Iterator last` ，表示区间维护的区间尾。（开区间）

2. 时间复杂度

    $O(n)$ 。

3. 备注

   同上。

   使用迭代器进行重置，可以将区间初状态直接赋到表里。

#### 4.查询区间元素数(query)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的区间的左端点。
   
   输入参数 `size_type right` ，表示要查询的区间的右端点。（闭区间）
   
   输入参数 `const KeyType &x` ，表示要查询的元素。
   
   返回类型 `size_type` ，表示查询到的结果。

2. 时间复杂度

   $O(\log n)$ 。


#### 三、模板示例

```c++
#include "DS/GlobalRangeCounter.h"
#include "IO/FastIO.h"

OY::GlobalRangeCounter<std::string, 1009> GRC;
void test() {
    // 本模板功能很简单，不多介绍
    std::string arr[5] = {"hello", "app", "apple", "hello", "app"};
    GRC.reset(arr, arr + 5);

    cout << "\"app\"  count of arr[1~3] = " << GRC.query(1, 3, "app") << endl;
    cout << "\"app\"  count of arr[1~4] = " << GRC.query(1, 4, "app") << endl;
    cout << "\"hell\" count of arr[0~4] = " << GRC.query(0, 4, "hell") << endl;
}

int main() {
    test();
}
```

```
#输出如下
"app"  count of arr[1~3] = 1
"app"  count of arr[1~4] = 2
"hell" count of arr[0~4] = 0

```

