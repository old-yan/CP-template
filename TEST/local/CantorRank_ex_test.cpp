#include "IO/FastIO.h"
#include "MATH/CantorRank_ex.h"
#include "MATH/StaticModInt32.h"

using mint = OY::mint1000000007;
int main() {
    // 较大范围的数字类型需要离散化
    long long B[] = {33333333333333, 33333333333333, 22222222222222, 33333333333333, 11111111111111};
    cout << "B's rank is " << OY::CantorRank_ex<mint, 100>().query(B, B + 5) << endl;
}
/*
#输出如下
B's rank is 17

*/