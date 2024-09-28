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
/*
#输出如下
points in [1000~2000] = 2
points in [1000~3000, 40~50] = 4

*/