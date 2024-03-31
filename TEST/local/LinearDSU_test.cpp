#include "DS/LinearDSU.h"
#include "IO/FastIO.h"

int main() {
    // 建立大小为 10 的并查集
    OY::LDSU::Table u(10);

    // 查询 3 和 6 的关系
    cout << "3 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    u.unite_after(3);
    u.unite_after(5);
    u.unite_after(8);
    u.unite_after(4);
    u.unite_after(6);
    // 查询 6 所在的分组首领
    cout << "6 is now in whose group:" << u.find_tail(6) << endl;
    // 查询 3 所在的分组首领
    cout << "3 is now in whose group:" << u.find_tail(3) << endl;
    // 查询 5 和 6 的关系
    cout << "3 and 6 in same group?  " << (u.in_same_group(3, 6) ? "true" : "false") << endl;

    auto tails = u.tails();
    for (uint32_t a : tails)
        cout << a << " is a tail\n";
}
/*
#输出如下
3 and 6 in same group?  false
6 is now in whose group:7
3 is now in whose group:7
3 and 6 in same group?  true
0 is a tail
1 is a tail
2 is a tail
7 is a tail
9 is a tail

*/