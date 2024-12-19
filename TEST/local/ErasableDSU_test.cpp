#include "DS/ErasableDSU.h"
#include "IO/FastIO.h"

int main() {
    OY::EDSU::Table u(6);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    u.unite_to(0, 4);
    u.unite_to(1, 4);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    u.unite_to(4, 2);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    cout << "extract 4 to " << u.extract(4) << endl;
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];

    u.unite_to(2, 6);
    for (int i = 0; i < 6; i++) cout << u.find(i) << " \n"[i == 5];
}
/*
#输出如下
0 1 2 3 4 5
4 4 2 3 4 5
2 2 2 3 2 5
extract 4 to 6
2 2 2 3 6 5
6 6 6 3 6 5

*/