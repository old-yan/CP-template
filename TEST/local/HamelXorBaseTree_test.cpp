#include "IO/FastIO.h"
#include "MATH/HamelXorBaseTree.h"
#include "TREE/LinkTree.h"

#include <bitset>

int main() {
    // 建树
    OY::LinkTree::Tree<bool, 1000> S(6);
    S.add_edge(2, 1);
    S.add_edge(1, 0);
    S.add_edge(1, 4);
    S.add_edge(4, 3);
    S.add_edge(4, 5);
    S.prepare(), S.set_root(2);

    // 不妨假定编号 i 处的向量为 1 << i

    OY::StaticHamelXorBaseTree32<8> hxb(&S, [](int i) { return 1 << i; });

    // 看看 1 到 5 的路径上的线性基
    hxb.enumerate_base(1, 5, [](uint32_t base) {
        cout << "base in path(1 -> 5): " << std::bitset<8>(base).to_string() << endl;
    });
    cout << endl;

    // 把 3 到 5 的路径的线性基单独拿出来
    auto hxb_3_to_5 = hxb.to_base_type(3, 5, 4);
    hxb_3_to_5.enumerate_base([](uint32_t base) {
        cout << "base in path(3 -> 5): " << std::bitset<8>(base).to_string() << endl;
    });
    cout << endl;
}
/*
#输出如下
base in path(1 -> 5): 00000010
base in path(1 -> 5): 00010000
base in path(1 -> 5): 00100000

base in path(3 -> 5): 00100000
base in path(3 -> 5): 00010000
base in path(3 -> 5): 00001000

*/