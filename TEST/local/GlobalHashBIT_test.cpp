#include "DS/GlobalHashBIT.h"
#include "IO/FastIO.h"

void test() {
    // 本模板完全等同于普通树状数组
    OY::GHashBIT<uint64_t, int64_t, 10, true> Gbit(100000000000000);
    Gbit.add(100, 9999);
    Gbit.add(1000000100, -99999);
    cout << Gbit.query(99, 1000000100) << endl;
    Gbit.clear();
    cout << Gbit.query_all() << endl;
}

int main() {
    test();
}
/*
#输出如下
-90000
0

*/