#include "IO/FastIO.h"
#include "MISC/LazyUset.h"

void test_key_add() {
    // key_type 可以全局加
    OY::LazyKeyAddUset<int> mp;
    mp.insert(100);
    mp.insert(300);
    mp.globally_add(10);
    mp.insert(200);
    mp.globally_add(2);

    cout << mp << "\n\n";
}

void test_key_mul() {
    // 设计一个 key 可以全局乘 的 set<double>

    // 需要写一个交换群
    struct monoid {
        using value_type = double;
        static double identity() { return 1; }
        static double op(double x, double y) { return x * y; }
        static double inverse(double x) { return 1 / x; }
    };
    using T = OY::LAZYUSET::Table<OY::LAZYIMP::LazyType<monoid>>;

    // 试试效果
    T mp;
    mp.insert(10.1);
    mp.insert(30.3);
    mp.globally_add(4);
    mp.insert(25);
    mp.globally_add(0.1);

    cout << mp << "\n\n";
}

int main() {
    test_key_add();
    test_key_mul();
}
/*
#输出如下
{202, 312, 112}

{2.500000, 12.120000, 4.040000}

*/