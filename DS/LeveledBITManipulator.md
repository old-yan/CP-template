### 一、模板类别

​	数据结构： 树状数组套数据结构控制器。

​	练习题目：

1. [P1903 [国家集训队] 数颜色 / 维护队列](https://www.luogu.com.cn/problem/P1903)
2. [P3157 [CQOI2011] 动态逆序对](https://www.luogu.com.cn/problem/P3157)
3. [P3332 [ZJOI2013] K大数查询](https://www.luogu.com.cn/problem/P3332)
4. [P3759 [TJOI2017] 不勤劳的图书管理员](https://www.luogu.com.cn/problem/P3759)
5. [速度即转发](https://ac.nowcoder.com/acm/problem/217863)



### 二、模板功能

​		本模板为树套树控制器。

​		本模板本身不提供修改、查询的具体功能，而只是嵌套在一个另外的数据结构 `InnerTable` 上，将 `InnerTable` 的修改和查询分发下去。

​		 `resize` 函数表示维护多大的区间。

​		 `modify_in_tables` 函数表示在所有包含下标 `i` 的表里进行同步的修改。

​		 `query_in_table` 函数表示在下标 `i` 的表里进行查询。

​		 `query_in_tables` 函数表示在一些表里进行查询，这些表所代表的区间拼凑出了 `[left, right]` 区间的信息。

​		 `max_right` 函数表示从下标 `0` 起二分。需要注意的是，二分分两个阶段，`judge` 函数本身不对已贪的区间信息进行合并，所以需要在 `judge` 返回 `true` （也就是可以把当前的区间贪到）时，将信息合并。


### 三、模板示例

```c++
#include "DS/LeveledBITManipulator.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

void test() {
    // LBM 和 LZM 大致相同，只不过更擅长用差分处理信息
    // 假如想维护二维平面上的点
    using base_table = OY::SEGCNT::Table<uint32_t, uint32_t, true, true, true>;
    OY::LBM::Table<base_table> S(10000);

    // 插入点 [1000,50]
    S.modify_in_tables(1000, [](base_table &table) {
        table.add(50, 1);
    });

    // 插入点 [2000,20]
    S.modify_in_tables(2000, [](base_table &table) {
        table.add(20, 1);
    });

    // 插入三个点 [3000,40]
    S.modify_in_tables(3000, [](base_table &table) {
        table.add(40, 3);
    });

    // 插入点 [4000,30]
    S.modify_in_tables(4000, [](base_table &table) {
        table.add(30, 1);
    });

    // 查询横坐标 [1000, 2000] 的总点数
    int tot = 0;
    auto plus_call = [&](const base_table &table) {
        tot += table.query_all();
    };
    auto minus_call = [&](const base_table &table) {
        tot -= table.query_all();
    };
    S.query_in_tables(1000, 2000, plus_call, minus_call);
    cout << "points in [1000~2000] = " << tot << endl;

    // 查询横坐标 [1000, 3000] ，纵坐标 [40, 50] 的总点数
    tot = 0;
    auto plus_call2 = [&](const base_table &table) {
        tot += table.query(40,50);
    };
    auto minus_call2 = [&](const base_table &table) {
        tot -= table.query(40,50);
    };
    S.query_in_tables(1000, 3000, plus_call2, minus_call2);
    cout << "points in [1000~3000, 40~50] = " << tot << endl;
}

int main() {
    test();
}
```

```
#输出如下
points in [1000~2000] = 2
points in [1000~3000, 40~50] = 4

```

