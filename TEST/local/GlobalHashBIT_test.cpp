#include "DS/GlobalHashBIT.h"
#include "IO/FastIO.h"

void test() {
    // 本模板完全等同于普通树状数组
    OY::GHashBIT<uint64_t, int64_t, true, true, 10> Gbit(100000000000000);
    Gbit.add(100, 9999);
    Gbit.add(101, 200, 1);
    Gbit.add(1000000100, -99999);
    cout << Gbit.query(100, 1000000100) << endl;
    Gbit.clear();
    cout << Gbit.query_all() << endl;
}

int main() {
    test();
}
/*
#输出如下
-89900
0

*/