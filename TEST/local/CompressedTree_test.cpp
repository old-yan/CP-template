#include "DS/CompressedTree.h"
#include "IO/FastIO.h"

void test() {
    // Compressed 和 Seg 相同之处不再展示
    // 展示一个单点修改的求乘积树的写法
    int A[] = {4, 9, 2, 3, 5, 7, 8, 1, 6};
    // 幺元为 1
    struct Monoid {
        using value_type = uint32_t;
        static value_type identity() { return 1; }
        static value_type op(value_type x, value_type y) { return x * y; }
    };
    OY::CPTREE::Tree<Monoid> S(A, A + 9);
    cout << S << endl;
    S.modify(3, 100);
    cout << S << endl;
    cout << "prod = " << S.query_all() << endl;
}

int main() {
    test();
}
/*
#输出如下
[0:4,1:9,2:2,3:3,4:5,5:7,6:8,7:1,8:6]
[0:4,1:9,2:2,3:100,4:5,5:7,6:8,7:1,8:6]
prod = 12096000

*/