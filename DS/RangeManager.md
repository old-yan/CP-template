### 一、模板类别

​	数据结构：区间管理。

### 二、模板功能

​	本数据结构继承了 `std::map` ，用键值对保存一个区间的左端点和右端点。

#### 1.建堆

1. 数据类型

   模板参数 `typename _Tp` ，表示区间的数据类型。

   构造参数可以使用 `std::map<_Tp,_Tp>` 的任何构造参数。

2. 时间复杂度

   $O(1)$ 。

3. 备注

   本数据结构中， `m_length` 属性会动态维护当前所有区间的长度之和。

#### 2.插入区间

1. 数据类型

   输入参数 `std::pair<_Tp,_Tp> __range` ，表示新插入的区间。

   返回类型 `std::pair<std::vector<std::pair<_Tp,_Tp>>, std::map<_Tp,_Tp>::iterator>` ，前者表示所有被新区间影响、进而删除的旧区间；后者将所有旧区间合并在一块，生成的新区间。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保参数左右端点合法。

#### 3.移除区间

1. 数据类型

   输入参数 `std::pair<_Tp,_Tp> __range` ，表示要移除的区间。

   返回类型 `std::pair<std::vector<std::pair<_Tp,_Tp>>, std::vector<std::pair<_Tp,_Tp>>>` ，前者表示所有与要移除的区间有牵扯、进而被删除的旧区间；后者为旧区间把要移除部分移除后，剩余的区间部分。

2. 时间复杂度

   均摊 $O(\log n)$ 。

3. 备注

   本函数没有进行参数检查，所以请自己确保参数左右端点合法。

#### 4.查询区间是否出现过

1. 数据类型

   输入参数 `const std::pair<_Tp,_Tp>& __range` ，表示要查询的区间。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   只要要查询的区间中，有任何一个数字已经存在，则返回 `true` 。
   
   本函数没有进行参数检查，所以请自己确保参数左右端点合法。

#### 5.查询区间是否全部存在

1. 数据类型

   输入参数 `const std::pair<_Tp,_Tp>& __range` ，表示要查询的区间。

2. 时间复杂度

   $O(\log n)$ 。

3. 备注

   仅当要查询的区间中，每个数字都已经存在，才会返回 `true` 。
   
   本函数没有进行参数检查，所以请自己确保参数左右端点合法。


### 三、模板示例

```c++
#include "DS/RangeManager.h"
#include "IO/FastIO.h"

int main() {
    OY::RangeManager<int> ranges;
    //添加区间
    ranges.addRange({1, 10});
    ranges.addRange({21, 30});
    ranges.addRange({5, 15});
    cout << "after addRange:" << ranges << endl;

    //移除区间
    auto [deleted, inserted] = ranges.removeRange({13, 25});
    cout << "after removeRange:" << ranges << endl;

    cout << "during removing,\n";
    for (auto &[l, r] : deleted) {
        cout << '[' << l << ',' << r << "] is deleted\n";
    }
    for (auto &[l, r] : inserted) {
        cout << '[' << l << ',' << r << "] is inserted\n";
    }

    //维护区间总长度
    cout << "length of ranges = " << ranges.m_length << endl;
}
```

```
#输出如下
after addRange:[[1, 15], [21, 30]]
after removeRange:[[1, 12], [26, 30]]
during removing,
[1,15] is deleted
[21,30] is deleted
[1,12] is inserted
[26,30] is inserted
length of ranges = 17

```

