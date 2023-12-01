#include "IO/FastIO.h"
#include "MATH/ModularInverseTable.h"
#include "MATH/StaticModInt32.h"

int main() {
    using mint = OY::mint1000000007;

    int A[3] = {1000, 2000, 3000};
    auto inverse_table = OY::get_modular_inverse_table<mint>(A, A + 3);
    for (int i = 0; i < 3; i++) {
        mint a = A[i];
        mint inv = inverse_table[i];
        cout << a << " * " << inv << " = " << a * inv << endl;
    }
}
/*
#输出如下
1000 * 857000006 = 1
2000 * 428500003 = 1
3000 * 952333340 = 1

*/