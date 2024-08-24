#include "DS/DynamicBitset.h"
#include "IO/FastIO.h"
#include "MISC/SubsetSumKnapsack.h"

int main() {
    constexpr int n = 100000;
    OY::DynamicBitset table(n + 1);
    std::vector<int> materials;
    for (int v = 40000; v <= 49999; v++) materials.push_back(v);
    for (int v = 60001; v <= 99999; v++) materials.push_back(v);
    OY::SSK::solve_01(table, materials);

    if (table[99999]) {
        cout << "99999 can be made\n";
    }
    if (!table[100000]) {
        cout << "100000 can not be made\n";
    }
}
/*
#输出如下
99999 can be made
100000 can not be made

*/