#include "GRAPH/Dijkstra_naive.h"
#include "IO/FastIO.h"
#include "TEST/std_bit.h"

void test_distance_sum() {
    // 普通使用者只需要了解熟悉 OY::DijkstraNaive::Graph 的使用
    cout << "test distance sum:\n";

    // 建图
    OY::VectorAddDijkstraNaive<int> G(7);
    // 注意加的边都是有向边
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 200);
    G.add_edge(3, 4, 100);
    G.add_edge(3, 5, 100);
    G.add_edge(0, 3, 95);
    G.add_edge(6, 4, 100);
    G.add_edge(4, 5, 190);
    G.add_edge(5, 1, 100);
    G.add_edge(5, 6, 200);

    // 获取最短路长度查询器
    auto table = G.calc(0);
    cout << "min dis from 0 to 0:" << table.query(0) << endl;
    cout << "min dis from 0 to 2:" << table.query(2) << endl;
    cout << "min dis from 0 to 6:" << table.query(6) << endl;

    // 如果模板参数为 true，那么查询器还可以查询最短路的结点编号

    auto table2 = G.calc<void, true>(0);
    table2.trace(6, [](int from, int to) { cout << "go from " << from << " -> " << to << endl; });

    // G 本身有更方便的接口
    std::vector<uint32_t> path = G.get_path(0, 6);
    for (int i = 0; i < path.size(); i++) cout << path[i] << (i + 1 == path.size() ? "\n\n" : " -> ");
}

void test_distance_max() {
    cout << "test distance max:\n";

    OY::VectorMaxDijkstraNaive<int> G(7);
    G.add_edge(0, 1, 100);
    G.add_edge(0, 2, 200);
    G.add_edge(3, 4, 100);
    G.add_edge(3, 5, 100);
    G.add_edge(0, 3, 95);
    G.add_edge(6, 4, 100);
    G.add_edge(4, 5, 190);
    G.add_edge(5, 1, 100);
    G.add_edge(5, 6, 200);

    // 定义路径长度为路径中的边长的最大值
    // 获取最短路查询器
    auto table = G.calc(0);
    cout << "min dis from 0 to 0:" << table.query(0) << endl;
    cout << "min dis from 0 to 2:" << table.query(2) << endl;
    cout << "min dis from 0 to 6:" << table.query(6) << endl;
    cout << endl;
}

void test_count() {
    cout << "test path count:\n";

    OY::VectorAddDijkstraNaive<int> G(4);
    G.add_edge(0, 1, 100);
    G.add_edge(1, 2, 200);
    G.add_edge(2, 3, 100);
    G.add_edge(0, 2, 300);
    G.add_edge(1, 3, 300);

    // 获取最短路路径数查询器
    auto table = G.calc<int>(0);
    cout << "min dis from 0 to 3:" << table.query(3) << endl;
    cout << "path count:" << table.query_count(3) << endl;
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
min dis from 0 to 0:0
min dis from 0 to 2:200
min dis from 0 to 6:395
go from 0 -> 3
go from 3 -> 5
go from 5 -> 6
0 -> 3 -> 5 -> 6

test distance max:
min dis from 0 to 0:0
min dis from 0 to 2:200
min dis from 0 to 6:200

test path count:
min dis from 0 to 3:400
path count:3

*/