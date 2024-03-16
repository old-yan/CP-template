#include "IO/FastIO.h"
#include "MATH/CatalanNumber.h"
#include "MATH/StaticModInt64.h"

using mint = OY::StaticModInt64<4611686018427387847, true>;
int main() {
    // 卡特兰数可以求合法括号序列及所有可以转换为合法括号序列模型的问题
    auto catalan_table = OY::CatalanNumber<mint, 30>(30);
    cout << "valid number of 3 pairs of brackets: " << catalan_table[3] << endl;
    cout << "valid number of 10 pairs of brackets: " << catalan_table[10] << endl;
    cout << "valid number of 20 pairs of brackets: " << catalan_table[20] << endl;
    cout << "valid number of 30 pairs of brackets: " << catalan_table[30] << endl;
}
/*
#输出如下
valid number of 3 pairs of brackets: 5
valid number of 10 pairs of brackets: 16796
valid number of 20 pairs of brackets: 6564120420
valid number of 30 pairs of brackets: 3814986502092304

*/