#include "IO/FastIO.h"
#include "MISC/LazyMap.h"

void test_key_add() {
    // key_type 可以全局加
    OY::LazyKeyAddMap<int, std::string> mp;
    mp.insert_or_assign(100, "A");
    mp.insert_or_assign(300, "C");
    mp.globally_add(10);
    mp.insert_or_assign(200, "B");
    mp.globally_add(2);

    cout << mp << "\n\n";
}

void test_mapped_add() {
    // mapped_type 可以全局加
    OY::LazyMappedAddMap<std::string, int> mp;
    mp.insert_or_assign("A", 100);
    mp.insert_or_assign("C", 300);
    mp.globally_add(10);
    mp.insert_or_assign("B", 200);
    mp.globally_add(2);

    cout << mp << "\n\n";
}

void test_key_add_mapped_add() {
    // key_type 和 mapped_type 都可以全局加
    OY::LazyKeyAddMappedAddMap<int, int> mp1;
    mp1.insert_or_assign(100, 1000);
    mp1.globally_add({20, 0});
    mp1.insert_or_assign(200, 2000);
    mp1.globally_add({0, 3});

    OY::LazyKeyAddMappedAddMap<int, int> mp2;
    mp2.insert_or_assign(400, 4000);
    mp2.globally_add({0, 5});
    mp2.insert_or_assign(300, 3000);
    mp2.globally_add({40, 0});
    mp2.insert_or_assign(200, 70000);

    // map 合并
    mp2.enumerate([&](int k, int v) {
        mp1.insert_or_modify(k, [&](int &old, bool flag) {
            if (flag)
                old = v;
            else
                old += v;
        });
    });
    mp2.clear();

    // 看看结果
    cout << mp1 << "\n\n";
}

void test_key_xor_mapped_mul() {
    // 设计一个 key 可以全局异或， mapped 可以全局乘 的 map<int, double>

    // key 支持异或很简单，有现成的
    using Key = OY::LAZYMAP::LazyBitXor<int>;

    // mapped 支持乘法需要写一个交换群
    struct monoid {
        using value_type = double;
        static double identity() { return 1; }
        static double op(double x, double y) { return x * y; }
        static double inverse(double x) { return 1 / x; }
    };
    using Mapped = OY::LAZYMAP::LazyType<monoid>;

    // 组合成一个 map
    using T = OY::LAZYMAP::Table<Key, Mapped>;

    // 试试效果
    T mp;
    mp.insert_or_assign(18, 10.1);
    mp.insert_or_assign(32, 30.3);
    mp.globally_add({4, 1});
    mp.insert_or_assign(25, 0.5);
    mp.globally_add({0, 0.1});
    mp.globally_add({12, 1});

    cout << mp << "\n\n";
}

void test_map_map() {
    // 二维 map
    // key_type 和 mapped_type 都可以全局加
    using Map1 = OY::LazyKeyAddMappedAddMap<int, int>;
    using Map2 = OY::LAZYMAP::Table<OY::LAZYMAP::LazyAdd<int>, Map1>;
    Map2 mp;
    mp.insert_or_assign(10, {});
    mp.insert_or_assign(20, {});
    mp.globally_add({1});
    mp.get(20).insert_or_assign(200, 2000);
    mp.insert_or_assign(30, {});
    mp.globally_add({2, {10, 0}});
    mp.get(10).insert_or_assign(100, 1000);

    cout << mp << "\n\n";
}

int main() {
    test_key_add();
    test_mapped_add();
    test_key_add_mapped_add();
    test_key_xor_mapped_mul();
    test_map_map();
}
/*
#输出如下
{112: A, 202: B, 312: C}

{A: 112, B: 202, C: 312}

{120: 1003, 200: 72003, 340: 3000, 440: 4005}

{26: 1.010000, 21: 0.050000, 40: 3.030000}

{10: {100: 1000}, 13: {}, 22: {210: 2000}, 23: {}, 32: {}}

*/