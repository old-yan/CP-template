#include "IO/FastIO.h"
#include "MATH/HamelXorBaseTable.h"

#include <bitset>

int main() {
    // 不妨假定编号 i 处的向量为 1 << i

    OY::StaticHamelXorBaseTable32<10> hxb(10, [](int i) { return 1 << i; });

    // 看看区间 [3, 5] 的线性基
    hxb.enumerate_base(3, 5, [](uint32_t base) {
        cout << "base in [3, 5]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;

    // 把 [0, 2] 的线性基单独拿出来
    auto hxb_0_to_2 = hxb.to_base_type(0, 2);
    hxb_0_to_2.enumerate_base([](uint32_t base) {
        cout << "base in [0, 2]: " << std::bitset<10>(base).to_string() << endl;
    });
    cout << endl;
}
/*
#输出如下
base in [3, 5]: 0000100000
base in [3, 5]: 0000010000
base in [3, 5]: 0000001000

base in [0, 2]: 0000000100
base in [0, 2]: 0000000010
base in [0, 2]: 0000000001

*/