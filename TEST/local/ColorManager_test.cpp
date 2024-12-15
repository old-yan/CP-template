#include "DS/ColorManager.h"
#include "IO/FastIO.h"

int main() {
    OY::CM::Table cm(10);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cm.modify(1, 6);
    cm.modify(2, 6);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cm.modify(6, 2);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cm.modify(3, 6);
    for (int i = 0; i < 10; i++) cout << cm.query(i) << " \n"[i == 9];

    cout << "item count of color2: " << cm.query_count(2) << endl;
}
/*
#输出如下
0 1 2 3 4 5 6 7 8 9
0 6 6 3 4 5 6 7 8 9
0 2 2 3 4 5 2 7 8 9
0 2 2 6 4 5 2 7 8 9
item count of color2: 3

*/