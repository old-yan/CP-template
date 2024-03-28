#include "GRAPH/Twosat.h"
#include "IO/FastIO.h"

int main() {
    // 六个变量，五条约束
    OY::TWOSAT::Graph G(6, 5);
    // 加五条约束
    G.add_clause(0, true, 1, true);
    G.add_clause(0, true, 2, false);
    G.add_clause(0, false, 3, true);
    G.add_clause(1, false, 5, false);
    G.add_clause(2, false, 3, true);
    // 计算可能性
    if (!G.calc()) {
        cout << "It's impossible\n";
    } else {
        cout << "It's possible\n";
        for (int i = 0; i < 6; i++) {
            cout << i << "'s value:" << (G.query(i) ? "true\n" : "false\n");
        }
    }
}
/*
#输出如下
It's possible
0's value:false
1's value:true
2's value:false
3's value:true
4's value:false
5's value:false

*/