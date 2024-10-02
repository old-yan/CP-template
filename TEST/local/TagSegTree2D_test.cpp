#include "DS/TagSegTree2D.h"
#include "IO/FastIO.h"

void test_sum_tree() {
    // 最可能用到的二维可区域加的求和树
    cout << "test sum tree:\n";

    using SumTree = OY::VectorTagSumSeg2D<int64_t>;
    SumTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 1);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

void test_max_tree() {
    // 可能用到的二维可区域最大化的最值树，默认值设为 -1
    cout << "test max tree:\n";

    using MaxTree = OY::VectorTagMaxSeg2D<int64_t, -1>;
    MaxTree S(10000000000, 10000000000);
    cout << S.query_all() << endl;

    S.add(0, 8000000000, 0, 8000000000, 100);
    cout << S.query(7500000000, 9000000000, 7500000000, 9000000000) << endl
         << endl;
}

void test_prod_tree() {
    // 展示用法的二维可区域乘的求积树
    cout << "test prod tree:\n";

    struct MulMonoid {
        using value_type = double;
        static value_type op(value_type x, value_type y) { return x * y; }
        static value_type identity() { return 1; }
        static value_type pow(value_type x, uint64_t n) { return ::pow(x, n); }
    };
    using ProdTree = OY::TagSEG2D::Tree<MulMonoid>;
    ProdTree S(5, 7);
    cout.precision(1);
    cout << S << endl;
    cout << S.query_all() << endl;

    S.add(1, 3, 1, 4, 2);
    cout << S << endl;
    cout << S.query(0, 2, 0, 2) << endl;
}

int main() {
    test_sum_tree();
    test_max_tree();
    test_prod_tree();
}
/*
#输出如下
test sum tree:
0
250000001000000001

test max tree:
-1
100

test prod tree:
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
1.0
[[1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 2.0, 2.0, 2.0, 2.0, 1.0, 1.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]]
16.0

*/