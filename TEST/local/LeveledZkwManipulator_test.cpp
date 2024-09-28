#include "DS/LeveledZkwManipulator.h"
#include "DS/SegCounter.h"
#include "IO/FastIO.h"

#include <set>

void test() {
    // 假如想维护二维平面上的点
    using base_table = OY::SEGCNT::Table<uint32_t, uint32_t, true, true, true>;
    OY::LZM::Table<base_table> S(10000);

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
    S.query_in_tables(1000, 2000, [&](const base_table &table) {
        tot += table.query_all();
    });
    cout << "points in [1000~2000] = " << tot << endl;

    // 查询横坐标 [1000, 3000] ，纵坐标 [40, 50] 的总点数
    tot = 0;
    S.query_in_tables(1000, 3000, [&](const base_table &table) {
        tot += table.query(40, 50);
    });
    cout << "points in [1000~3000, 40~50] = " << tot << endl;
}

void test2() {
    // base_table 还可以是各种 stl 容器
    using base_table = std::set<int>;
    OY::LZM::Table<base_table> S(10000);

    auto add_point = [&](int x, int y) {
        S.modify_in_tables(x, [&](base_table &table) { table.insert(y); });
    };
    add_point(1000, 50);
    add_point(2000, 45);
    add_point(3000, 60);
    add_point(1500, 46);
    add_point(4000, 56);

    // 找到 [1000,3000] 范围内，纵坐标大于 45 的最小值
    int lb = -1;
    S.query_in_tables(1000, 3000, [&](const base_table &table) {
        auto it = table.upper_bound(45);
        if (it != table.end() and (lb == -1 or *it < lb)) lb = *it;
    });
    cout << "res = " << lb << endl;

    // 找到从 1000 往右，第一个纵坐标达到 55 的点，返回其位置
    int val = -1;
    // 二分函数的意思是，检查是否满足最大值小于 55
    int pos = S.max_right(1000, [&](const base_table &table) {
        if (table.empty()) return true;
        auto mx = *table.rbegin();
        if (mx >= 55) val = mx;
        return mx < 55;
    }) + 1;
    cout << "res = (" << pos << ", " << val << ")" << endl;
}

int main() {
    test();
    test2();
}
/*
#输出如下
points in [1000~2000] = 2
points in [1000~3000, 40~50] = 4
res = 46
res = (3000, 60)

*/