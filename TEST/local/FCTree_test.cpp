#include "DS/FCTree.h"
#include "IO/FastIO.h"

void test() {
    int perm[] = {4, 9, 2, 3, 5, 7, 8, 1, 6, 0};
    auto mapping = [](int i) { return i * 10; };
    auto perm_mapping = [&](int i) { return perm[i]; };
    
    OY::FCTree<int> S(10, mapping, perm_mapping);
    for (int i = 0; i < 10; i++) cout << S.query(i) << " \n"[i == 9];

    S.add(2, 6, 1000);
    for (int i = 0; i < 10; i++) cout << S.query(i) << " \n"[i == 9];
}

void brute_force() {
    int perm[] = {4, 9, 2, 3, 5, 7, 8, 1, 6, 0};
    auto mapping = [](int i) { return i * 10; };
    
    int S[10]{};
    for (int i = 0; i < 10; i++) S[i] = mapping(i);
    for (int i = 0; i < 10; i++) cout << S[perm[i]] << " \n"[i == 9];

    for (int i = 2; i <= 6; i++) S[i] += 1000;
    for (int i = 0; i < 10; i++) cout << S[perm[i]] << " \n"[i == 9];
}

int main() {
    test();
    brute_force();
}
/*
#输出如下
40 90 20 30 50 70 80 10 60 0
1040 90 1020 1030 1050 70 80 10 1060 0
40 90 20 30 50 70 80 10 60 0
1040 90 1020 1030 1050 70 80 10 1060 0

*/