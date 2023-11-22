#include "IO/FastIO.h"
#include "MATH/CantorSequence.h"

int main() {
    for (int k = 0; k < 6; k++) {
        auto v = OY::get_Cantor_sequence(3, k);
        cout << k << "-th permutation of [0,1,2]: ";
        for (auto a : v) cout << a << ' ';
        cout << endl;
    }
}
/*
#输出如下
0-th permutation of [0,1,2]: 0 1 2 
1-th permutation of [0,1,2]: 0 2 1 
2-th permutation of [0,1,2]: 1 0 2 
3-th permutation of [0,1,2]: 1 2 0 
4-th permutation of [0,1,2]: 2 0 1 
5-th permutation of [0,1,2]: 2 1 0 

*/