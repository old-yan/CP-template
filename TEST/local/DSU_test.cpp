#include "DS/DSU.h"
#include "IO/FastIO.h"

int main() {
    // 建立大小为 10 的并查集
    OY::DSUTable<true> u(10);

    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    u.unite_by_ID(2, 5);
    u.unite_by_ID(3, 6);
    u.unite_by_ID(1, 9);
    u.unite_by_ID(7, 8);
    u.unite_by_ID(2, 3);
    // 查询 6 所在的分组首领
    cout << "6 is now in whose group:" << u.find(6) << endl;
    // 查询 6 所在的分组大小
    cout << "6's group size is:      " << u.size(6) << endl;
    // 查询 5 和 6 的关系
    cout << "5 and 6 in same group?  " << (u.in_same_group(5, 6) ? "true" : "false") << endl;

    auto heads = u.heads();
    for (uint32_t a : heads)
        cout << a << " is a head\n";

    auto groups = u.groups();
    for (uint32_t i = 0; i < groups.size(); i++) {
        cout << "No." << i + 1 << " group's member:";
        for (uint32_t a : groups[i])
            cout << a << ' ';
        cout << endl;
    }
}
/*
#输出如下
5 and 6 in same group?  false
6 is now in whose group:2
6's group size is:      4
5 and 6 in same group?  true
0 is a head
1 is a head
2 is a head
4 is a head
7 is a head
No.1 group's member:0 
No.2 group's member:1 9 
No.3 group's member:2 3 5 6 
No.4 group's member:4 
No.5 group's member:7 8 
*/