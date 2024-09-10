### 一、模板类别

​	数据结构：区间管理。

​	练习题目：

1. [P1503 鬼子进村](https://www.luogu.com.cn/problem/P1503)
2. [P1840 Color the Axis](https://www.luogu.com.cn/problem/P1840)
3. [P2082 区间覆盖（加强版）](https://www.luogu.com.cn/problem/P2082)
4. [P2434 [SDOI2005] 区间](https://www.luogu.com.cn/problem/P2434)
5. [P3201 [HNOI2009] 梦幻布丁](https://www.luogu.com.cn/problem/P3201)
6. [P3740 [HAOI2014] 贴海报](https://www.luogu.com.cn/problem/P3740)
7. [P4979 矿洞：坍塌](https://www.luogu.com.cn/problem/P4979)
8. [U282660 区间合并](https://www.luogu.com.cn/problem/U282660)
9. [J. Range Sets](https://qoj.ac/contest/1399/problem/7641)


### 二、模板功能

​	本数据结构基于 `std::map` ，用键值对保存一个区间的左端点和右端点。

#### 1.建立区间管理

1. 数据类型

   模板参数 `typename Tp` ，表示区间的数据类型。

2. 时间复杂度

   $O(1)$ 。

#### 2.插入区间(add_range)

1. 数据类型

   输入参数 `std::pair<Tp, Tp> range` ，表示新插入的区间。

   输入参数 `AddCallback &&add_call`，表示在插入一个区间时调用的回调函数。默认为 `Ignore` 类对象。

   输入参数 `RemoveCallback &&remove_call`，表示在移除一个区间时调用的回调函数。默认为 `Ignore` 类对象。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   尽管这是 `add_range` 函数，但是由于区间会合并，所以也会有移除区间的行为发生。
   

#### 3.移除区间(remove_range)

1. 数据类型

   输入参数 `std::pair<Tp, Tp> range` ，表示要移除的区间。

   输入参数 `AddCallback &&add_call`，表示在插入一个区间时调用的回调函数。默认为 `Ignore` 类对象。

   输入参数 `RemoveCallback &&remove_call`，表示在移除一个区间时调用的回调函数。默认为 `Ignore` 类对象。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   尽管这是 `remove_range` 函数，同样会有插入区间的行为发生。

#### 4.查询区间是否出现过(any_of)

1. 数据类型

   输入参数 `const std::pair<Tp, Tp> &range` ，表示要查询的区间。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   只要要查询的区间中，有任何一个数字已经存在，则返回最先出现的、与 `range` 相交的迭代器；否则返回 `end` 。
   
   本函数没有进行参数检查，所以请自己确保参数左右端点合法。

#### 5.查询区间是否全部存在(all_of)

1. 数据类型

   输入参数 `const std::pair<Tp, Tp> &range` ，表示要查询的区间。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   仅当要查询的区间中，每个数字都已经存在，才会返回包含 `range` 的迭代器；否则返回 `end` 。
   
   本函数没有进行参数检查，所以请自己确保参数左右端点合法。


#### 6.获取区间数量(size)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 7.获取区间总场地(length)

1. 数据类型

2. 时间复杂度

   $O(1)$ 。

#### 8.清空区间(clear)

1. 数据类型

2. 时间复杂度

   $O(n)$ 。

#### 9.获取左端点大于等于某值的区间(lower_bound)

1. 数据类型

   输入参数 `const Tp &key`, 表示要大于等于的值。

2. 时间复杂度

   $O(\log n)$ 。

#### 10.获取左端点大于某值的区间(upper_bound)

1. 数据类型

   输入参数 `const Tp &key`, 表示要大于的值。

2. 时间复杂度

   $O(\log n)$ 。

### 三、模板示例

```c++
#include "DS/RangeManager.h"
#include "IO/FastIO.h"

int main() {
    OY::RangeManager<int> ranges;

    // 一般来说，回调可以写成匿名函数，而且 add_range 和 remove_range 的回调通用
    auto add_call = [&](uint32_t l, uint32_t r) {
        cout << "range [" << l << ", " << r << "] is added\n";
    };
    auto remove_call = [&](uint32_t l, uint32_t r) {
        cout << "range [" << l << ", " << r << "] is removed\n";
    };

    // 添加区间
    ranges.add_range({1, 10}, add_call, remove_call);
    ranges.add_range({21, 30}, add_call, remove_call);
    ranges.add_range({5, 15}, add_call, remove_call);
    cout << "after add_range:" << ranges << endl;

    // 移除区间
    ranges.remove_range({13, 25}, add_call, remove_call);
    cout << "after remove_range:" << ranges << endl;

    // 维护区间总长度
    cout << "length of ranges = " << ranges.length() << endl;
}
```

```
#输出如下
range [1, 10] is added
range [21, 30] is added
range [1, 10] is removed
range [1, 15] is added
after add_range:[[1, 15], [21, 30]]
range [1, 15] is removed
range [1, 12] is added
range [21, 30] is removed
range [26, 30] is added
after remove_range:[[1, 12], [26, 30]]
length of ranges = 17

```

