#include "IO/FastIO.h"
#include "MATH/CantorSequence_ex.h"

int main() {
    std::string s = "acbc";
    for (int k = 0; k < 12; k++) {
        auto v = OY::get_Cantor_sequence_ex(s.begin(), s.end(), k);
        cout << k << "-th permutation of \"acbc\": ";
        for (auto a : v) cout << a;
        cout << endl;
    }
}
/*
#输出如下
0-th permutation of "acbc": abcc
1-th permutation of "acbc": acbc
2-th permutation of "acbc": accb
3-th permutation of "acbc": bacc
4-th permutation of "acbc": bcac
5-th permutation of "acbc": bcca
6-th permutation of "acbc": cabc
7-th permutation of "acbc": cacb
8-th permutation of "acbc": cbac
9-th permutation of "acbc": cbca
10-th permutation of "acbc": ccab
11-th permutation of "acbc": ccba

*/