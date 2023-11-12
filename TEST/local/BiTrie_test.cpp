#include "DS/BiTrie.h"
#include "IO/FastIO.h"

void test_normal() {
    OY::BiTrie32<5, OY::BiTrie::BaseInfo, 1000> S;

    // 插入数字
    S.insert(15);
    S.insert(1);
    S.insert(9);

    // 插入数字的返回值是一个迭代器，可以拿来做操作
    auto insert_leaf = S.insert(14);

    // 查询和 5 有最大相同值的数
    auto res1 = S.query_max_same(5);
    auto leaf1 = res1.first;
    auto mask1 = res1.second;
    cout << "5 and some number's"
         << " common bitmask :" << mask1 << endl;
    // 查询最大异或
    auto res2 = S.query_max_bitxor(5);
    auto leaf2 = res2.first;
    auto mask2 = res2.second;
    cout << "5 and some number's"
         << " bitxor result :" << mask2 << endl;
    S.erase(5 ^ mask2);
    auto res3 = S.query_max_bitxor(5);
    auto leaf3 = res3.first;
    auto mask3 = res3.second;
    cout << "5 and some number's"
         << " bitxor result :" << mask3 << endl;
}

void test_custom() {
    // 假定，现在我们在求最大异或、最大相同之外，还要添加条件: 要求必须在模 3 余数相同的群组内部进行查询
    struct Info {
        // 考虑 m_mask 的末三位。
        // 若 m_mask 有个位，说明子树中有模 3 余 0 的；
        // 若 m_mask 有十位，说明子树中有模 3 余 1 的；
        // 若 m_mask 有百位，说明子树中有模 3 余 2 的；
        uint32_t m_mask;
    };
    OY::BiTrie32<5, Info, 1000> S;
    using iterator = decltype(S)::iterator;
    // 插入数字时，记得修改一路上的 mask
    for (int val = 0; val < 10; val++) S.insert(val, [&](iterator it) { it->m_mask |= 1 << (val % 3); });
    // 在模 3 余数相同的范围内，查询 19 的最大异或
    auto res = S.query_max_bitxor(19, [&](iterator it) { return it && (it->m_mask >> (19 % 3) & 1); });
    auto leaf = res.first;
    auto mask = res.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask << endl;
    // 不限范围，查询 18 的最大异或
    auto res2 = S.query_max_bitxor(19, [&](iterator it) { return it; });
    auto leaf2 = res2.first;
    auto mask2 = res2.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask2 << endl;
    // 但是此时注意，字典树里删叶节点可不能乱删。得清空掉从叶到根的 m_mask 的残留影响。
    S.erase(19 ^ mask2);
    S.trace(19 ^ mask2, [](iterator p) {
        p->m_mask = p.child(0)->m_mask | p.child(1)->m_mask;
    });
    // 再次不限范围，查询 18 的最大异或。注意参数可以省略
    auto res3 = S.query_max_bitxor(19);
    auto leaf3 = res3.first;
    auto mask3 = res3.second;
    cout << "19 and some number's"
         << " bitxor result :" << mask3 << endl;
}

int main() {
    test_normal();
    test_custom();
}
/*
#输出如下
5 and some number's common bitmask :27
5 and some number's bitxor result :12
5 and some number's bitxor result :11
19 and some number's bitxor result :23
19 and some number's bitxor result :27
19 and some number's bitxor result :26

*/