#include "GRAPH/DijkstraKPath.h"
#include "GRAPH/SPFAKPath.h"
#include "IO/FastIO.h"
#include "TEST/std_bit.h"

void test_distance_sum() {
    // 普通使用者只需要了解熟悉 OY::SPFAKPath::Graph 的使用
    cout << "test distance sum:\n";

    // 建图
    OY::SPFAKPath::Graph<int> G(7, 10);
    // 注意加的边都是有向边
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 200);
    G.add_edge(2, 0, 1);
    G.add_edge(3, 4, 100);
    G.add_edge(3, 5, 100);
    G.add_edge(0, 3, 95);
    G.add_edge(6, 4, 100);
    G.add_edge(4, 5, 190);
    G.add_edge(5, 1, 100);
    G.add_edge(5, 6, 200);

    // 获取最短路，次短路长度查询器
    auto table = G.calc<2>(0).first;
    cout << "top2 dis from 0 to 0:" << table.query(0, 0) << ',' << table.query(0, 1) << endl;
    cout << "top2 dis from 0 to 2:" << table.query(2, 0) << ',' << table.query(2, 1) << endl;
    cout << "top2 dis from 0 to 6:" << table.query(6, 0) << ',' << table.query(6, 1) << endl;

    // 如果模板参数为 true，那么查询器还可以查询次短路的结点编号
    using group = OY::SPFAKPath::AddGroup<int>;

    auto table2 = G.calc<2, group, void, true>(0).first;
    // 最短路路径
    cout << "No.1 path:\n";
    table2.trace(6, 0, [](int from, int to) { cout << "go from " << from << " -> " << to << endl; });
    cout << "No.2 path:\n";
    table2.trace(6, 1, [](int from, int to) { cout << "go from " << from << " -> " << to << endl; });

    // G 本身有更方便的接口
    std::vector<uint32_t> path1 = G.get_path(0, 6, 0);
    for (int i = 0; i < path1.size(); i++) cout << path1[i] << (i + 1 == path1.size() ? "\n" : " -> ");
    std::vector<uint32_t> path2 = G.get_path(0, 6, 1);
    for (int i = 0; i < path2.size(); i++) cout << path2[i] << (i + 1 == path2.size() ? "\n\n" : " -> ");
}

void test_distance_max() {
    cout << "test distance max:\n";

    OY::SPFAKPath::Graph<int> G(7, 10);
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 200);
    G.add_edge(2, 0, 1);
    G.add_edge(3, 4, 100);
    G.add_edge(3, 5, 100);
    G.add_edge(0, 3, 95);
    G.add_edge(6, 4, 100);
    G.add_edge(4, 5, 190);
    G.add_edge(5, 1, 100);
    G.add_edge(5, 6, 200);

    // 定义路径长度为路径中的边长的最大值
    // 获取最短路查询器
    using group = OY::SPFAKPath::MaxGroup<int>;
    auto table = G.calc<2, group>(0).first;
    cout << "top2 dis from 0 to 0:" << table.query(0, 0) << ',' << table.query(0, 1) << endl;
    cout << "top2 dis from 0 to 2:" << table.query(2, 0) << ',' << table.query(2, 1) << endl;
    cout << "top2 dis from 0 to 6:" << table.query(6, 0) << ',' << table.query(6, 1) << endl;
    cout << endl;
}

void test_count() {
    cout << "test path count:\n";

    OY::SPFAKPath::Graph<int> G(4, 7);
    G.add_edge(0, 3, 100);
    G.add_edge(0, 2, 200);
    G.add_edge(0, 1, 120);
    G.add_edge(1, 2, 80);
    G.add_edge(2, 3, 300);
    G.add_edge(2, 3, 300);
    G.add_edge(2, 0, 1);

    // 获取最短路路径数查询器
    using monoid = OY::SPFAKPath::AddGroup<int>;
    auto table = G.calc<3, monoid, int>(0).first;
    cout << "No.1 dis from 0 to 3:" << table.query(3, 0) << endl;
    cout << "path count:" << table.query_count(3, 0) << endl;
    cout << "No.2 dis from 0 to 3:" << table.query(3, 1) << endl;
    cout << "path count:" << table.query_count(3, 1) << endl;
    cout << "No.3 dis from 0 to 3:" << table.query(3, 2) << endl;
    cout << "path count:" << table.query_count(3, 2) << endl;
    cout << endl;
}

int main() {
    test_distance_sum();
    test_distance_max();
    test_count();
}
/*
#输出如下
test distance sum:
top2 dis from 0 to 0:0,201
top2 dis from 0 to 2:200,401
top2 dis from 0 to 6:395,585
No.1 path:
go from 0 -> 3
go from 3 -> 5
go from 5 -> 6
No.2 path:
go from 0 -> 3
go from 3 -> 4
go from 4 -> 5
go from 5 -> 6
0 -> 3 -> 5 -> 6
0 -> 3 -> 4 -> 5 -> 6

test distance max:
top2 dis from 0 to 0:0,200
top2 dis from 0 to 2:200,200
top2 dis from 0 to 6:200,200

test path count:
No.1 dis from 0 to 3:100
path count:1
No.2 dis from 0 to 3:301
path count:2
No.3 dis from 0 to 3:500
path count:4

*/