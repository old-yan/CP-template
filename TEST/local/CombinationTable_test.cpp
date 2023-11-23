#include "IO/FastIO.h"
#include "MATH/CombinationTable.h"
#include "MATH/StaticModInt32.h"

using mint = OY::StaticModInt32<97, true>;
int main() {
    OY::CombinationTable<mint, 20> table;
    int people = 10;
    int need_people = 4;
    // 在 10 个人里抽出 4 个人，一起去参加某一场会议的方案数
    cout << "comb(" << people << "," << need_people << ") = " << table.comb(people, need_people) << endl;
    // 10 个人按序参加 10 场会议（每人一场）的方案数
    cout << "perm(" << people << "," << people << ") = " << table.perm(people) << endl;
    // 在 10 个人里抽出 4 个人，按序参加 4 场会议（每人一场）的方案数
    cout << "perm(" << people << "," << need_people << ") = " << table.perm(people, need_people) << endl;
    // 在 10 个人里抽出 4 个不相邻的人，一起去参加某一场会议的方案数
    cout << "nonadjacent perm(" << people << "," << need_people << ") = " << table.comb_nonadjacent(people, need_people) << endl;
    // 10 个人每个人有一个不想去参加的会议，则他们参加 10 场会议（每人一场）且皆大欢喜的总方案数
    cout << "staggered perm(" << people << "," << people << ") = " << table.perm_staggered(people) << endl;
    // 10 个人在圆桌上吃饭的排列总方案数
    cout << "circular perm(" << people << "," << people << ") = " << table.perm_circular(people) << endl;
    // 10 个人派 4 个人去圆桌上吃饭的排列总方案数
    cout << "circular perm(" << people << "," << need_people << ") = " << table.perm_circular(people, need_people) << endl;

    // 把 2 个苹果，3 个梨，5 个橘子排列的方案数
    int apple_cnt = 2;
    int pear_cnt = 3;
    int orange_cnt = 5;
    std::vector<int> items{apple_cnt, pear_cnt, orange_cnt};
    cout << "perm(apple * " << apple_cnt << ", pear * " << pear_cnt << ", orange * " << orange_cnt << ") = " << table.perm_multi(items.begin(), items.end()) << endl;
}
/*
#输出如下
comb(10,4) = 16
perm(10,10) = 30
perm(10,4) = 93
nonadjacent perm(10,4) = 35
staggered perm(10,10) = 47
circular perm(10,10) = 3
circular perm(10,4) = 96
perm(apple * 2, pear * 3, orange * 5) = 95

*/