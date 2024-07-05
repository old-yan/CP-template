#include "GRAPH/BFS.h"
#include "IO/FastIO.h"
#include "TEST/std_bit.h"

void test_bfs() {
    // 普通使用者只需要了解熟悉 OY::BFS::Graph 的使用
    cout << "test bfs:\n";

    // 建图
    OY::BFS::Graph G(7, 9);
    // 注意加的边都是有向边
    G.add_edge(0, 1);
    G.add_edge(0, 2);
    G.add_edge(3, 4);
    G.add_edge(0, 3);
    G.add_edge(6, 4);
    G.add_edge(4, 5);
    G.add_edge(5, 1);
    G.add_edge(3, 5);
    G.add_edge(5, 6);

    // 获取最短路长度查询器
    auto table = G.calc<false>(0);

    cout << "min dis from 0 to 0:" << table.query(0) << endl;
    cout << "min dis from 0 to 2:" << table.query(2) << endl;
    cout << "min dis from 0 to 6:" << table.query(6) << endl;

    // 如果模板参数为 true，那么查询器还可以查询最短路的结点编号
    auto table2 = G.calc<true>(0);
    table2.trace(6, [](int from, int to) {
        cout << "go from " << from << " -> " << to << endl;
    });

    // G 本身有更方便的接口
    std::vector<uint32_t> path = G.get_path(0, 6);
    for (int i = 0; i < path.size(); i++) cout << path[i] << (i + 1 == path.size() ? "\n\n" : " -> ");
}

void test_solver() {
#if CPP_STANDARD >= 201402L
    // 进阶使用者，可以把 Solver 用到自己的图里
    cout << "test solver:\n";
    // 这里以常见的二维 vector 存图举例
    std::vector<std::vector<int>> adj(7);
    adj[0].push_back(1);
    adj[0].push_back(2);
    adj[3].push_back(4);
    adj[0].push_back(3);
    adj[6].push_back(4);
    adj[4].push_back(5);
    adj[5].push_back(1);
    adj[3].push_back(5);
    adj[5].push_back(6);

    // 直接建一个可追溯最短路的解答器
    OY::BFS::Solver<true> sol(7, 9);
    sol.set_distance(0, 0);
    // 传递一个遍历边的泛型回调
    sol.run([&](int from, auto call) {
        for (int to : adj[from]) call(to);
    });

    // 查询最短路长度
    cout << "min dis from 0 to 0:" << sol.query(0) << endl;
    cout << "min dis from 0 to 2:" << sol.query(2) << endl;
    cout << "min dis from 0 to 6:" << sol.query(6) << endl;

    // 生成一个最短路径
    sol.trace(6, [](int from, int to) {
        cout << "from " << from << " to " << to << endl;
    });

#endif
}

int main() {
    test_bfs();
    test_solver();
}
/*
#输出如下
test bfs:
min dis from 0 to 0:0
min dis from 0 to 2:1
min dis from 0 to 6:3
go from 0 -> 3
go from 3 -> 5
go from 5 -> 6
0 -> 3 -> 5 -> 6

test solver:
min dis from 0 to 0:0
min dis from 0 to 2:1
min dis from 0 to 6:3
from 0 to 3
from 3 to 5
from 5 to 6

*/