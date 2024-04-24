#include "DS/LinearDSU.h"
#include "IO/FastIO.h"

int main() {
    // 建立大小为 10 的并查集
    OY::LDSU::Table<true> u(10);

    // 查询 3 和 6 的关系
    cout << "3 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    u.unite_after(3);
    u.unite_after(5);
    u.unite_after(8);
    u.unite_after(4);
    u.unite_after(6);
    cout << u << endl;
    // 查询 6 所在的分组首元素和尾元素
    cout << "6 is now in which group:" << u.find_head(6) << "~" << u.find_tail(6) << endl;
    // 查询 3 所在的分组首领
    cout << "3 is now in which group:" << u.find_head(3) << "~" << u.find_tail(3) << endl;
    // 查询 5 和 6 的关系
    cout << "3 and 6 in same group?  " << (u.in_same_group(3, 6) ? "true" : "false") << endl;

    auto heads = u.heads();
    for (uint32_t a : heads)
        cout << a << " is a head\n";

    auto tails = u.tails();
    for (uint32_t a : tails)
        cout << a << " is a tail\n";
}
/*
#输出如下
3 and 6 in same group?  false
[[0, 0], [1, 1], [2, 2], [3, 7], [8, 9]]
6 is now in which group:3~7
3 is now in which group:3~7
3 and 6 in same group?  true
0 is a head
1 is a head
2 is a head
3 is a head
8 is a head
0 is a tail
1 is a tail
2 is a tail
7 is a tail
9 is a tail

*/