#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

#include <bitset>

void test_static() {
    // 维护一个最大位长为 8 的异或线性基
    OY::StaticHamelXorBase32<8> hxb{};
    // 插入向量
    hxb.insert(0b00001011);
    hxb.insert(0b01101001);
    hxb.insert(0b01101010);
    hxb.insert(0b01100001);

    // 输出基向量个数
    cout << "base count = " << hxb.base_count() << endl;

    // 枚举所有的基向量
    uint32_t index = 0;
    hxb.enumerate_base([&](uint32_t base) {
        cout << "No." << index++ << " base: " << std::bitset<8>(base).to_string() << endl;
    });
    cout << endl;

    // 输出包含/可表达的向量个数
    cout << "total count = " << hxb.total_count() << endl;

    // 枚举所有的包含/可表达的向量，注意这个枚举并不按照大小顺序
    index = 0;
    hxb.enumerate([&](uint32_t mask) {
        cout << "No." << index++ << " mask: " << std::bitset<8>(mask).to_string() << endl;
    });
    cout << endl;

    // 通过第二个和第三个基向量，可以拼凑出 0b1000
    if (hxb.contains(0b1000))
        cout << "0b1000 is contained\n";
    else
        cout << "0b1000 is not contained\n";

    // 无法拼凑出 0b100
    if (hxb.contains(0b100))
        cout << "0b100  is contained\n";
    else
        cout << "0b100  is not contained\n";

    // 查询第 k 大向量
    for (int k = 0; k < hxb.total_count(); k++)
        cout << "No." << k << " mask: " << std::bitset<8>(hxb.kth(k)).to_string() << endl;
    cout << endl;

    // 查询每个向量的排名
    hxb.enumerate([&](uint32_t mask) {
        cout << "mask" << std::bitset<8>(mask).to_string() << "'s rank = " << hxb.rank(mask) << endl;
    });
    cout << endl;

    // 查询是否绑定
    // 显然，第 5 位和第 6 位永远是一样的值
    if (hxb.is_bonded(5, 6)) cout << "No.5 digit and No.6 digit is bonded\n";
    // 第 0 位和第 1 位可以是不同的值
    if (!hxb.is_bonded(0, 1)) cout << "No.0 digit and No.1 digit is not bonded\n";
    cout << endl;

    // 用的最多的，还是查询最大异或
    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor()).to_string() << endl;

    // 有的时候会强制要求从某个向量处出发，找最大异或
    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor(0b1000000)).to_string() << endl;

    cout << endl;
}

void test_dynamic() {
    // 动态维度的异或线性基，必须在使用前指定维度
    OY::DynamicHamelXorBase32::set_width(8);
    OY::DynamicHamelXorBase32 hxb;

    // 插入向量
    hxb.insert(0b00001011);
    hxb.insert(0b01101001);
    hxb.insert(0b01101010);
    hxb.insert(0b01100001);

    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor()).to_string() << endl;
    cout << "max bitxor = " << std::bitset<8>(hxb.query_max_bitxor(0b1000000)).to_string() << endl;
}

int main() {
    test_static();
    test_dynamic();
}
/*
#输出如下
base count = 3
No.0 base: 01101001
No.1 base: 00001011
No.2 base: 00000011

total count = 8
No.0 mask: 00000000
No.1 mask: 00000011
No.2 mask: 00001011
No.3 mask: 00001000
No.4 mask: 01101001
No.5 mask: 01101010
No.6 mask: 01100010
No.7 mask: 01100001

0b1000 is contained
0b100  is not contained
No.0 mask: 00000000
No.1 mask: 00000011
No.2 mask: 00001000
No.3 mask: 00001011
No.4 mask: 01100001
No.5 mask: 01100010
No.6 mask: 01101001
No.7 mask: 01101010

mask00000000's rank = 0
mask00000011's rank = 1
mask00001011's rank = 3
mask00001000's rank = 2
mask01101001's rank = 6
mask01101010's rank = 7
mask01100010's rank = 5
mask01100001's rank = 4

No.5 digit and No.6 digit is bonded
No.0 digit and No.1 digit is not bonded

max bitxor = 01101010
max bitxor = 01001011

max bitxor = 01101010
max bitxor = 01001011

*/