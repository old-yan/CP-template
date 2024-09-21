#include "DS/FHQCounter.h"
#include "IO/FastIO.h"

void test() {
    using Counter = OY::FHQCNT::Table<uint32_t, int, false, false>;
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

    S2 = S1.split_by_key(3);
    cout << "after split by 3:\n";
    cout << "S1: " << S1 << endl;
    cout << "S2: " << S2 << endl;

    // 枚举 S1 对象
    auto call = [](uint32_t k, int v) {
        cout << "S1[" << k << "] == " << v << endl;
    };
    S1.enumerate(call);
    cout << endl;
}

void test_range_query() {
    using Counter = OY::FHQCNT::Table<uint64_t, int, true, true>;
    Counter S;
    S.add(1, 200);
    S.add(3999999, 100);
    S.add(500000000000, 50);
    S.add(3999999, 10);
    S.add(4999999, 8);
    cout << "S: " << S << endl;

    cout << "size = " << S.size() << endl;
    cout << "tot = " << S.query_all() << endl;
    cout << "S.query(3999999, 4999999) = " << S.query(3999999, 4999999) << endl;
    cout << "S.kth(0) = " << S.kth(0)->m_key << endl;
    cout << "S.kth(199) = " << S.kth(199)->m_key << endl;
    cout << "S.kth(200) = " << S.kth(200)->m_key << endl;
    cout << "S.kth(317) = " << S.kth(317)->m_key << endl;
    cout << "S.kth(318) = " << S.kth(318)->m_key << endl;
    cout << "S.kth(367) = " << S.kth(367)->m_key << endl
         << endl;
}

int main() {
    test();
    test_range_query();
}
/*
#输出如下
S1: {1*200,3*100,5*50}
S2: {2*70,3*10,6*60}
after merge:
S1: {1*200,2*70,3*110,5*50,6*60}
S2: {}
after split by 3:
S1: {1*200,2*70}
S2: {3*110,5*50,6*60}
S1[1] == 200
S1[2] == 70

S: {1*200,3999999*110,4999999*8,500000000000*50}
size = 4
tot = 368
S.query(3999999, 4999999) = 118
S.kth(0) = 1
S.kth(199) = 1
S.kth(200) = 3999999
S.kth(317) = 4999999
S.kth(318) = 500000000000
S.kth(367) = 500000000000

*/
