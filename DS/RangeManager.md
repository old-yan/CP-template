### 一、模板类别

​	数据结构：区间管理。

​	练习题目：

1. [U282660 区间合并](https://www.luogu.com.cn/problem/U282660)

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

   返回类型 `std::pair<std::vector<std::pair<Tp, Tp>>, std::map<Tp, Tp>::iterator>` ，前者表示所有被新区间影响、进而删除的旧区间；后者将所有旧区间合并在一块，生成的新区间。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保参数左右端点合法。

#### 3.移除区间(remove_range)

1. 数据类型

   输入参数 `std::pair<Tp, Tp> range` ，表示要移除的区间。

   返回类型 `std::pair<std::vector<std::pair<Tp, Tp>>, std::vector<std::pair<Tp, Tp>>>` ，前者表示所有与要移除的区间有牵扯、进而被删除的旧区间；后者为旧区间把要移除部分移除后，剩余的区间部分。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保参数左右端点合法。

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
    // 添加区间
    ranges.add_range({1, 10});
    ranges.add_range({21, 30});
    ranges.add_range({5, 15});
    cout << "after add_range:" << ranges << endl;

    // 移除区间
    // 结构化绑定需要 C++17
#if CPP_STANDARD >= 201702L
    auto [deleted, inserted] = ranges.remove_range({13, 25});
#else
    auto deleted_and_inserted = ranges.remove_range({13, 25});
    auto deleted = deleted_and_inserted.first;
    auto inserted = deleted_and_inserted.second;
#endif
    cout << "after remove_range:" << ranges << endl;

    cout << "during removing,\n";
    // 结构化绑定需要 C++17
#if CPP_STANDARD >= 201702L
    for (auto &[l, r] : deleted) {
        cout << '[' << l << ',' << r << "] is deleted\n";
    }
    for (auto &[l, r] : inserted) {
        cout << '[' << l << ',' << r << "] is inserted\n";
    }
#else
    for (auto &l_and_r : deleted) {
        auto l = l_and_r.first, r = l_and_r.second;
        cout << '[' << l << ',' << r << "] is deleted\n";
    }
    for (auto &l_and_r : inserted) {
        auto l = l_and_r.first, r = l_and_r.second;
        cout << '[' << l << ',' << r << "] is inserted\n";
    }
#endif

    // 维护区间总长度
    cout << "length of ranges = " << ranges.m_length << endl;
}
```

```
#输出如下
after add_range:[[1, 15], [21, 30]]
after remove_range:[[1, 12], [26, 30]]
during removing,
[1,15] is deleted
[21,30] is deleted
[1,12] is inserted
[26,30] is inserted
length of ranges = 17

```

