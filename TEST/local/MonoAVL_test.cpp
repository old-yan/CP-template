#include "DS/MonoAVL.h"
#include "IO/FastIO.h"

void test() {
    cout << "test of no pushup:\n";
    OY::MonoAVLSequence<int, true> S;
    S.insert(0, 100);
    S.insert(1, 30);
    S.insert(1, 50);
    S.insert(1, 120);
    cout << "S : " << S << endl;

    S.reverse(1, 2);
    cout << "S : " << S << endl;
    S.insert(4, 70);
    cout << "S : " << S << endl;
    S.modify(1, 11);
    cout << "S : " << S << endl;
    S.erase(2);
    cout << "S : " << S << endl;

    auto S2 = OY::MonoAVLSequence<int, true>::from_mapping(3, [&](int i) { return (i + 1) * 40; });
    cout << "S2 : " << S2 << endl;

    S2.join(S.split(2));
    cout << "S : " << S << endl;
    cout << "S2 : " << S2 << endl
         << endl;
}

void test_fast_reverse() {
    cout << "test of fast reverse:\n";
    struct Monoid {
        using value_type = int;
        using sum_type = int64_t;
        static value_type identity() { return 1; }
        static sum_type op(sum_type x, sum_type y) { return x * y; }
        static sum_type reversed(sum_type x) { return x; }
    };
    OY::MONOAVL::Tree<Monoid, true> S;
    cout << "prod = " << S.query_all() << endl;

    S.insert(0, 100);
    S.insert(1, 30);
    S.insert(1, 50);
    S.insert(1, 120);
    cout << "S : " << S << endl;
    cout << "prod = " << S.query_all() << endl;
    S.reverse(1, 2);
    cout << "S : " << S << endl;
    S.insert(4, 70);
    cout << "S : " << S << endl;
    S.modify(1, 11);
    cout << "S : " << S << endl;
    S.erase(2);
    cout << "S : " << S << endl;
    cout << "prod(S[1~2]) = " << S.query(1, 2) << endl
         << endl;
}

void test_slow_reverse() {
    cout << "test of slow reverse:\n";
    struct Monoid {
        using value_type = std::string;
        static value_type identity() { return ""; }
        static value_type op(value_type x, value_type y) { return x + y; }
    };
    OY::MONOAVL::Tree<Monoid, true> S;

    S.insert(0, "a");
    S.insert(1, "c");
    S.insert(1, "f");
    S.insert(1, "e");
    S.insert(3, "d");
    S.insert(0, "b");
    cout << "S : " << S << endl;
    cout << "prod = " << S.query_all() << endl;
    S.reverse(1, 3);
    cout << "S : " << S << endl;
    S.modify(1, "z");
    cout << "S : " << S << endl;
    cout << "prod(S[0~2]) = " << S.query(0, 2) << endl;
    cout << "prod(S[1~4]) = " << S.query(1, 4) << endl;
    S.reverse(0, 5);
    cout << "S : " << S << endl;
    cout << "prod(S[1~4]) = " << S.query(1, 4) << endl;
}

int main() {
    test();
    test_fast_reverse();
    test_slow_reverse();
}
/*
#输出如下
test of no pushup:
S : {100, 120, 50, 30}
S : {100, 50, 120, 30}
S : {100, 50, 120, 30, 70}
S : {100, 11, 120, 30, 70}
S : {100, 11, 30, 70}
S2 : {40, 80, 120}
S : {100, 11}
S2 : {40, 80, 120, 30, 70}

test of fast reverse:
prod = 1
S : {100, 120, 50, 30}
prod = 18000000
S : {100, 50, 120, 30}
S : {100, 50, 120, 30, 70}
S : {100, 11, 120, 30, 70}
S : {100, 11, 30, 70}
prod(S[1~2]) = 330

test of slow reverse:
S : {b, a, e, f, d, c}
prod = baefdc
S : {b, f, e, a, d, c}
S : {b, z, e, a, d, c}
prod(S[0~2]) = bze
prod(S[1~4]) = zead
S : {c, d, a, e, z, b}
prod(S[1~4]) = daez

*/