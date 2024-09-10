#include "DS/SegCounter.h"
#include "IO/FastIO.h"

void test() {
    using Counter = OY::SEGCNT::Table<uint32_t, int, false, false, false>;
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
    using Counter = OY::SEGCNT::Table<uint64_t, int, true, true, false>;
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
    cout << "S.kth(0) = " << S.kth(0)->key() << endl;
    cout << "S.kth(199) = " << S.kth(199)->key() << endl;
    cout << "S.kth(200) = " << S.kth(200)->key() << endl;
    cout << "S.kth(317) = " << S.kth(317)->key() << endl;
    cout << "S.kth(318) = " << S.kth(318)->key() << endl;
    cout << "S.kth(367) = " << S.kth(367)->key() << endl
         << endl;
}

void test_bitxor() {
    using Counter = OY::SEGCNT::Table<uint64_t, int, true, false, false>;
    Counter S;
    S.add(31, 1);
    S.add(35, 5);
    S.add(48, 10);
    S.add(55, 50);
    S.add(57, 100);
    S.add(60, 500);
    S.add(62, 1000);
    cout << "S: " << S << endl;

    // 寻找和 21 异或结果最小的结点
    auto p = S.min_bitxor(21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果最大的结点
    p = S.max_bitxor(21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果略小于 40 的结点
    p = S.smaller_bound_bitxor(40, 21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果略大于 40 的结点
    p = S.upper_bound_bitxor(40, 21);
    cout << p->key() << " ^ 21 = " << (p->key() ^ 21) << endl;

    // 寻找和 21 异或结果位于 [35, 45] 的结点的值的和
    auto cnt_35_45 = S.query_bitxor(35, 45, 21);
    cout << "sum(35 <= (key^21) <=45) = " << cnt_35_45 << endl;

    // 异或意义下进行分裂
    auto S2 = S.split_by_key_bitxor(43, 21);
    // 验证 S 的键和 21 的异或，肯定小于 43
    cout << "S: " << S << endl;
    S.enumerate([](uint64_t key, int cnt) {
        cout << key << " ^ 21 = " << (key ^ 21) << endl;
    });

    // 验证 S2 的键和 21 的异或，肯定大于等于 43
    cout << "S2: " << S2 << endl;
    S2.enumerate([](uint64_t key, int cnt) {
        cout << key << " ^ 21 = " << (key ^ 21) << endl;
    });
}

int main() {
    test();
    test_range_query();
    test_bitxor();
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

S: {31*1,35*5,48*10,55*50,57*100,60*500,62*1000}
31 ^ 21 = 10
35 ^ 21 = 54
48 ^ 21 = 37
60 ^ 21 = 41
sum(35 <= (key^21) <=45) = 1610
S: {31*1,48*10,55*50,60*500}
31 ^ 21 = 10
48 ^ 21 = 37
55 ^ 21 = 34
60 ^ 21 = 41
S2: {35*5,57*100,62*1000}
35 ^ 21 = 54
57 ^ 21 = 44
62 ^ 21 = 43

*/
