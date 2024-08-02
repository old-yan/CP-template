#include "GRAPH/Psuedotree_ug.h"
#include "IO/FastIO.h"

void test_cycle() {
    // 建立一个多个环图，看看 cycle 是怎么划分的
    OY::PsuedoUG::Graph<bool> G(9);
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);
    G.add_edge(3, 0);
    G.add_edge(4, 4);
    G.add_edge(5, 7);
    G.add_edge(6, 7);
    G.add_edge(7, 8);
    G.add_edge(8, 6);
    G.prepare();

    cout << "there are " << G.cycle_count() << " cycles\n";
    for (int id = 0; id < G.cycle_count(); id++) {
        auto cycle = G.get_cycle_info(id);
        cout << "No." << id + 1 << " cycle:\n";
        cout << "cycle size = " << cycle.size() << endl;
        cout << "component size = " << cycle.component_size() << endl;
        cout << "sequences: ";
        for (auto v : cycle) cout << v << ' ';
        cout << endl;
    }
    cout << endl;
}

void test_tree() {
    // 建立一个单环图，看看环外树是怎么维护的
    OY::PsuedoUG::Graph<bool, uint32_t, true> G(20);
    // 10~19 形成环
    for (int i = 10; i <= 19; i++) G.add_edge(i, i == 19 ? 10 : i + 1);
    // 0~5 形成一棵树，依附在环上的结点 11 之下
    G.add_edge(0, 1);
    G.add_edge(2, 1);
    G.add_edge(3, 4);
    G.add_edge(5, 4);
    G.add_edge(1, 11);
    G.add_edge(4, 11);

    // 6~9 形成一条链，依附在环上的结点 14 之下
    G.add_edge(9, 8);
    G.add_edge(8, 7);
    G.add_edge(7, 6);
    G.add_edge(6, 14);
    G.prepare();

    for (int step = 0; step <= 20; step++)
        cout << "from 9 walk " << step << (step > 1 ? " steps:" : " step :") << G.next(9, step) << endl;

    cout << "lca of 2 and 5 = " << G.join(2, 5) << endl;
    cout << "lca of 2 and 0 = " << G.join(2, 0) << endl;

    // 2 在环外树上，8 也在环外树上
    auto cycle = G.get_cycle_info(0);
    cout << "distance from 2 to cycle = " << G.get_vertex_info(2).step_to_cycle() << endl;
    cout << "distance from 8 to cycle = " << G.get_vertex_info(8).step_to_cycle() << endl;

    // 如果 2 和 8 相会在 8 的入环口，分别要走多少步？
    cout << "to join at 14:\n";
    auto join_2 = G.get_vertex_info(2).cycle_join();
    auto join_8 = G.get_vertex_info(8).cycle_join();
    cout << "2 needs to walk " << G.get_vertex_info(2).step_to_cycle() + cycle.position(join_8) - cycle.position(join_2) << endl;
    cout << "8 needs to walk " << G.get_vertex_info(8).step_to_cycle() << endl;
    // 如果 2 和 8 相会在 2 的入环口，分别要走多少步？
    cout << "to join at 11:\n";
    cout << "2 needs to walk " << G.get_vertex_info(2).step_to_cycle() << endl;
    cout << "8 needs to walk " << G.get_vertex_info(8).step_to_cycle() + (cycle.length() + cycle.position(join_2) - cycle.position(join_8)) << endl;
}

int main() {
    test_cycle();
    test_tree();
}
/*
#输出如下
there are 3 cycles
No.1 cycle:
cycle size = 4
component size = 4
sequences: 0 1 2 3 
No.2 cycle:
cycle size = 1
component size = 1
sequences: 4 
No.3 cycle:
cycle size = 3
component size = 3
sequences: 6 7 8 

from 9 walk 0 step :9
from 9 walk 1 step :8
from 9 walk 2 steps:7
from 9 walk 3 steps:6
from 9 walk 4 steps:14
from 9 walk 5 steps:15
from 9 walk 6 steps:16
from 9 walk 7 steps:17
from 9 walk 8 steps:18
from 9 walk 9 steps:19
from 9 walk 10 steps:10
from 9 walk 11 steps:11
from 9 walk 12 steps:12
from 9 walk 13 steps:13
from 9 walk 14 steps:14
from 9 walk 15 steps:15
from 9 walk 16 steps:16
from 9 walk 17 steps:17
from 9 walk 18 steps:18
from 9 walk 19 steps:19
from 9 walk 20 steps:10
lca of 2 and 5 = 11
lca of 2 and 0 = 1
distance from 2 to cycle = 2
distance from 8 to cycle = 3
to join at 14:
2 needs to walk 5
8 needs to walk 3
to join at 11:
2 needs to walk 2
8 needs to walk 10

*/