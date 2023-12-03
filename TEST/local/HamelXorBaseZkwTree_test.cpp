#include "IO/FastIO.h"
#include "MATH/HamelXorBaseZkwTree.h"

#include <bitset>

int main() {
    // 不妨假定编号 i 处的向量为 1 << i

    OY::StaticHamelXorBaseZkwTree32<10> hxb(10, [](int i) { return 1 << i; });

    // 看看区间 [0, 2] 的线性基
    hxb.enumerate_base(0, 2, [](uint32_t base) {
        cout << "base in [0, 2]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 看看区间 [3, 5] 的线性基
    hxb.enumerate_base(3, 5, [](uint32_t base) {
        cout << "base in [3, 5]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 现在我们给区间 [2, 3] 异或上 0b1000
    hxb.range_xor(2, 3, 0b1000);
    
    // 看看区间 [0, 2] 的线性基
    hxb.enumerate_base(0, 2, [](uint32_t base) {
        cout << "base in [0, 2]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 看看区间 [3, 5] 的线性基
    hxb.enumerate_base(3, 5, [](uint32_t base) {
        cout << "base in [3, 5]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;
}
/*
#输出如下
base in [0, 2]: 0000000001
base in [0, 2]: 0000000011
base in [0, 2]: 0000000110

base in [3, 5]: 0000001000
base in [3, 5]: 0000011000
base in [3, 5]: 0000110000

base in [0, 2]: 0000000001
base in [0, 2]: 0000000011
base in [0, 2]: 0000001110

base in [3, 5]: 0000010000
base in [3, 5]: 0000110000

*/