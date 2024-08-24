### 一、模板类别

​	数据结构：区间众数查询表

​	练习题目：

1. [P3709 大爷的字符串题](https://www.luogu.com.cn/problem/P3709)
2. [P4168 [Violet] 蒲公英](https://www.luogu.com.cn/problem/P4168)
3. [P5048 [Ynoi2019 模拟赛] Yuno loves sqrt technology III](https://www.luogu.com.cn/problem/P5048)
4. [Static Range Mode Query](https://judge.yosupo.jp/problem/static_range_mode_query)(https://github.com/yosupo06/library-checker-problems/issues/959)


### 二、模板功能

#### 1.建立众数表

1. 数据类型


   模板参数 `typename Tp` ，表示元素类型。
   
   模板参数 `bool Minimize` ，表示是否最小化众数元素。

   构造参数 `size_type length` ，表示区间长度。
   
   构造参数 `InitMapping mapping` ，表示从下标到区间内元素的映射函数。

2. 时间复杂度

   $O(n\cdot\sqrt n)$ 。

3. 备注

   当模板参数 `Minimize` 为 `false` 时，运行效率稍快；当模板参数 `Minimize` 为 `true` 时，可以支持查询最小众数。

#### 2.建立众数表

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

#### 5.查询众数(query)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的区间的左端点。
   
   输入参数 `size_type right` ，表示要查询的区间的右端点。（闭区间）
   
   返回类型 `Pair<Tp>` ，表示查询到的众数元素，及其出现次数。

2. 时间复杂度

   $O(\sqrt n)$ 。

#### 6.查询最小众数(query_min)

1. 数据类型

   输入参数 `size_type left` ，表示要查询的区间的左端点。
   
   输入参数 `size_type right` ，表示要查询的区间的右端点。（闭区间）
   
   返回类型 `Pair<Tp>` ，表示查询到的众数元素，及其出现次数。

2. 时间复杂度

   $O(\sqrt n)$ 。

3. 备注

   本函数仅当 `Minimize` 为 `true` 时可用。

#### 三、模板示例

```c++
#include "DS/RangeMode.h"
#include "IO/FastIO.h"

void test() {
    // 本模板功能很简单，不多介绍
    int arr[9] = {400, 900, 200, 300, 400, 300, 300, 100, 900};
    OY::RangeMode<int> rm(arr, arr + 9);

    cout << "mode of arr[1~3] = " << rm.query(1, 3).m_val << " for " << rm.query(1, 3).m_cnt << " times" << endl;
    cout << "mode of arr[4~6] = " << rm.query(4, 6).m_val << " for " << rm.query(4, 6).m_cnt << " times" << endl;
    cout << "mode of arr[1~6] = " << rm.query(1, 6).m_val << " for " << rm.query(1, 6).m_cnt << " times" << endl;
}

int main() {
    test();
}
```

```
#输出如下
mode of arr[1~3] = 900 for 1 times
mode of arr[4~6] = 300 for 2 times
mode of arr[1~6] = 300 for 3 times

```

