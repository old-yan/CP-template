### 一、模板类别

​	数据结构： 树套数据结构控制器。

​	练习题目：

1. [P1903 [国家集训队] 数颜色 / 维护队列](https://www.luogu.com.cn/problem/P1903)
2. [P3157 [CQOI2011] 动态逆序对](https://www.luogu.com.cn/problem/P3157)
3. [P3332 [ZJOI2013] K大数查询](https://www.luogu.com.cn/problem/P3332)
4. [P3759 [TJOI2017] 不勤劳的图书管理员](https://www.luogu.com.cn/problem/P3759)
5. [P8764 [蓝桥杯 2021 国 BC] 二进制问题](https://www.luogu.com.cn/problem/P8764)
6. [速度即转发](https://ac.nowcoder.com/acm/problem/217863)



### 二、模板功能

​		本模板为树套树控制器。本模板与 `LeveledZkwManipulator` 的作用是一样的，区别在于，本模板可以维护更大的区间；本模板内部并不包含数据结构，而只会在相应修改、查询的时候，把一些区间边界传递给使用者，而区间边界到具体的数据结构的映射关系的维护，交给使用者自己来。

​		 `resize` 函数表示维护多大的区间。

​		 `modify_in_tables` 函数表示在所有包含下标 `i` 的表里进行同步的修改。回调函数会对所有包含下标 `i` 的区间调用。

​		 `query_in_tables` 函数表示在一些表里进行查询，这些表所代表的区间拼凑出了 `[left, right]` 区间的信息。

​		 `max_right`  和 `min_left` 函数表示在区间上二分。需要注意的是，`judge` 函数本身不对已贪的区间信息进行合并，所以需要在 `judge` 返回 `true` （也就是可以把当前的区间贪到）时，将信息合并。


### 三、模板示例

```c++
#include "DS/LeveledSegManipulator.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

#include <map>
#include <set>

void test() {
    // 假如想维护二维平面上的点
    using base_table = OY::SEGCNT::Table<uint32_t, uint32_t, true, true, true>;
    OY::LSM32 S(10000);
    std::map<std::pair<uint32_t, uint32_t>, base_table> table_mp;

    // 插入点 [1000,50]
    S.modify_in_tables(1000, [&](uint32_t L, uint32_t R) {
        table_mp[{L, R}].add(50, 1);
    });

    // 插入点 [2000,20]
    S.modify_in_tables(2000, [&](uint32_t L, uint32_t R) {
        table_mp[{L, R}].add(20, 1);
    });

    // 插入三个点 [3000,40]
    S.modify_in_tables(3000, [&](uint32_t L, uint32_t R) {
        table_mp[{L, R}].add(40, 3);
    });

    // 插入点 [4000,30]
    S.modify_in_tables(4000, [&](uint32_t L, uint32_t R) {
        table_mp[{L, R}].add(30, 1);
    });

    // 查询横坐标 [1000, 2000] 的总点数
    int tot = 0;
    S.query_in_tables(1000, 2000, [&](uint32_t L, uint32_t R) {
        auto it = table_mp.find({L, R});
        if (it != table_mp.end())
            tot += it->second.query_all();
    });
    cout << "points in [1000~2000] = " << tot << endl;

    // 查询横坐标 [1000, 3000] ，纵坐标 [40, 50] 的总点数
    tot = 0;
    S.query_in_tables(1000, 3000, [&](uint32_t L, uint32_t R) {
        auto it = table_mp.find({L, R});
        if (it != table_mp.end())
            tot += it->second.query(40, 50);
    });
    cout << "points in [1000~3000, 40~50] = " << tot << endl;
}

void test2() {
    // base_table 还可以是各种 stl 容器
    using base_table = std::set<int>;
    OY::LSM32 S(10000);
    std::map<std::pair<uint32_t, uint32_t>, base_table> table_mp;

    auto add_point = [&](int x, int y) {
        S.modify_in_tables(x, [&](uint32_t L, uint32_t R) { table_mp[{L, R}].insert(y); });
    };
    add_point(1000, 50);
    add_point(2000, 45);
    add_point(3000, 60);
    add_point(1500, 46);
    add_point(4000, 56);

    // 找到 [1000,3000] 范围内，纵坐标大于 45 的最小值
    int lb = -1;
    S.query_in_tables(1000, 3000, [&](uint32_t L, uint32_t R) {
        auto table = table_mp.find({L, R});
        if (table == table_mp.end()) return;
        auto it = table->second.upper_bound(45);
        if (it != table->second.end() and (lb == -1 or *it < lb)) lb = *it;
    });
    cout << "res = " << lb << endl;

    // 找到从 1000 往右，第一个纵坐标达到 55 的点，返回其位置
    int val = -1;
    // 二分函数的意思是，检查是否满足最大值小于 55
    int pos = S.max_right(1000, [&](uint32_t L, uint32_t R) {
        auto table = table_mp.find({L, R});
        if (table == table_mp.end()) return true;
        if (table->second.empty()) return true;
        auto mx = *table->second.rbegin();
        if (mx >= 55) val = mx;
        return mx < 55;
    }) + 1;
    cout << "res = (" << pos << ", " << val << ")" << endl;
}

int main() {
    test();
    test2();
}
```

```
#输出如下
points in [1000~2000] = 2
points in [1000~3000, 40~50] = 4
res = 46
res = (3000, 60)

```

