#include "DS/ReversedSegCounter.h"
#include "IO/FastIO.h"

void test() {
    using Counter = OY::REVSEGCNT::Table<uint32_t, int, true, true, true>;
    Counter S1;
    S1.add(1, 200);
    S1.add(3, 100);
    S1.add(5, 50);
    cout << "S1: " << S1 << endl;

    Counter S2;
    S2.add(2, 70);
    S2.add(3, 10);
    S2.add(6, 60);
    cout << "S2: " << S2 << endl;

    S1.merge(S2);
    cout << "after merge:\n";
    cout << "S1: " << S1 << endl;
    cout << "S2: " << S2 << endl;

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
S1: {1*200,5*50,3*100}
S2: {2*70,6*60,3*10}
after merge:
S1: {2*70,6*60,1*200,5*50,3*110}
S2: {}
S1[2] == 70
S1[6] == 60
S1[1] == 200
S1[5] == 50
S1[3] == 110
after bitxor by 15:
S1: {12*110,10*50,14*200,9*60,13*70}
S1[12] == 110
S1[10] == 50
S1[14] == 200
S1[9] == 60
S1[13] == 70
after plus one:
S1: {10*60,14*70,13*110,11*50,15*200}
S1[10] == 60
S1[14] == 70
S1[13] == 110
S1[11] == 50
S1[15] == 200
after bitxor by 15:
S1: {0*200,4*50,2*110,1*70,5*60}
S1[0] == 200
S1[4] == 50
S1[2] == 110
S1[1] == 70
S1[5] == 60
after minus one:
S1: {0*70,4*60,1*110,3*50,2147483647*200}
S1[0] == 70
S1[4] == 60
S1[1] == 110
S1[3] == 50
S1[2147483647] == 200

it->key() = 3
after bitxor by 15: it->key() = 12
after plus one: it->key() = 13

*/