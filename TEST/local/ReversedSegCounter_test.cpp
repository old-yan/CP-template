#include "DS/ReversedSegCounter.h"
#include "IO/FastIO.h"

void test() {
    using Counter = OY::REVSEGCNT::Table<uint32_t, int, true, true, true>;
    Counter S1;
    S1.add(1, 201);
    S1.add(3, 103);
    S1.add(5, 55);
    cout << "S1: " << S1 << endl;
    cout << "key xor sum = " << S1.key_xorsum() << endl;
    cout << "mapped sum = " << S1.mapped_sum() << endl;

    Counter S2;
    S2.add(2, 71);
    S2.add(3, 11);
    S2.add(6, 60);
    cout << "S2: " << S2 << endl;
    cout << "key xor sum = " << S2.key_xorsum() << endl;
    cout << "mapped sum = " << S2.mapped_sum() << endl;

    S1.merge(S2);
    cout << "after merge:\n";
    cout << "S1: " << S1 << endl;
    cout << "key xor sum = " << S1.key_xorsum() << endl;
    cout << "mapped sum = " << S1.mapped_sum() << endl;
    cout << "S2: " << S2 << endl;
    cout << "key xor sum = " << S2.key_xorsum() << endl;
    cout << "mapped sum = " << S2.mapped_sum() << endl;

    // 枚举 S1 对象
    auto call = [](uint32_t k, int v) {
        cout << "S1[" << k << "] == " << v << endl;
    };
    S1.enumerate(call);

    S1.globally_bitxor(15);
    cout << "after bitxor by 15:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);

    S1.globally_plus_one();
    cout << "after plus one:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);

    S1.globally_bitxor(15);
    cout << "after bitxor by 15:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);

    S1.globally_minus_one();
    cout << "after minus one:\n";
    cout << "S1: " << S1 << endl;
    S1.enumerate(call);
    cout << endl;
}

void test_node_fetch() {
    // 可以根据结点指针，获取其最新值
    using Counter = OY::REVSEGCNT::Table<uint32_t, int, true, true, true>;
    Counter S;
    S.add(1, 200);
    S.add(3, 100);
    S.add(5, 50);

    auto it = S.find(3);
    cout << "it->key() = " << it->key() << endl;
    S.globally_bitxor(15);
    it->fetch();
    cout << "after bitxor by 15: it->key() = " << it->key() << endl;
    S.globally_plus_one();
    it->fetch();
    cout << "after plus one: it->key() = " << it->key() << endl;
}

int main() {
    test();
    test_node_fetch();
}
/*
#输出如下
S1: {1*201,5*55,3*103}
key xor sum = 7
mapped sum = 359
S2: {2*71,6*60,3*11}
key xor sum = 1
mapped sum = 142
after merge:
S1: {2*71,6*60,1*201,5*55,3*114}
key xor sum = 6
mapped sum = 501
S2: {}
key xor sum = 0
mapped sum = 0
S1[2] == 71
S1[6] == 60
S1[1] == 201
S1[5] == 55
S1[3] == 114
after bitxor by 15:
S1: {12*114,10*55,14*201,9*60,13*71}
S1[12] == 114
S1[10] == 55
S1[14] == 201
S1[9] == 60
S1[13] == 71
after plus one:
S1: {10*60,14*71,13*114,11*55,15*201}
S1[10] == 60
S1[14] == 71
S1[13] == 114
S1[11] == 55
S1[15] == 201
after bitxor by 15:
S1: {0*201,4*55,2*114,1*71,5*60}
S1[0] == 201
S1[4] == 55
S1[2] == 114
S1[1] == 71
S1[5] == 60
after minus one:
S1: {0*71,4*60,1*114,3*55,2147483647*201}
S1[0] == 71
S1[4] == 60
S1[1] == 114
S1[3] == 55
S1[2147483647] == 201

it->key() = 3
after bitxor by 15: it->key() = 12
after plus one: it->key() = 13

*/