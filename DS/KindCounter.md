### 一、模板类别

​	数据结构：区间元素种类数查询表

​	练习题目：

1. [P1972 [SDOI2009] HH的项链](https://www.luogu.com.cn/problem/P1972)
2. [U41492 树上数颜色](https://www.luogu.com.cn/problem/U41492)


### 二、模板功能

#### 1.建立种类表

1. 数据类型

   类型设定 `size_type = uint32_t` ，表示表中下标的类型。

   模板参数 `typename Tag` ，表示用于记录某个元素上次出现位置的哈希表类型。

   构造参数 `size_type length` ，表示区间长度。
   
   构造参数 `InitMapping mapping` ，表示从下标到区间内元素的映射函数。

2. 时间复杂度

   $O(n\cdot\log n)$ 。

3. 备注

   本模板内提供了三种可选的 `Tag` 。第一种是 `MapTag` ，顾名思义，提供 `std::map` 作为哈希表；第二种是 `UmapTag` ，顾名思义，提供 `std::unordered_map` 作为哈希表；第三种是 `ArrayTag` ，顾名思义，提供一个数组作为哈希表。显然，这三种哈希表各有优劣：第一种哈希表支持各种能比较大小的元素类型，但是时间复杂度较高；第二种哈希表时间复杂度低，支持能哈希的元素类型，但是当哈希冲突增多时效率下降；第三种哈希表仅支持元素类型为数字的情况，且要求数字的值域为正，且绝对值不能过大。
   
   使用者可以根据具体情况选择哈希表。

#### 2.建立种类表

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

#### 5.查询种类数(query)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的区间的左端点。
   
   输入参数 `size_type right` ，表示要查询的区间的右端点。（闭区间）
   
   返回类型 `size_type` ，表示查询到的种类数。

2. 时间复杂度

   $O(\log n)$ 。


#### 三、模板示例

```c++
#include "DS/KindCounter.h"
#include "IO/FastIO.h"

void test() {
    // 本模板功能很简单，不多介绍
    int arr[9] = {400, 900, 200, 300, 400, 300, 300, 100, 900};
    OY::ArrayKindCounter<900> kc(arr, arr + 9);

    cout << "kind count of arr[1~3] = " << kc.query(1, 3) << endl;
    cout << "kind count of arr[4~6] = " << kc.query(4, 6) << endl;
    cout << "kind count of arr[1~6] = " << kc.query(1, 6) << endl;
}

int main() {
    test();
}
```

```
#输出如下
kind count of arr[1~3] = 3
kind count of arr[4~6] = 2
kind count of arr[1~6] = 4

```

