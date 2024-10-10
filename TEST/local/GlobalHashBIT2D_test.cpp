#include "DS/GlobalHashBIT2D.h"
#include "IO/FastIO.h"

// 本模板用法完全等同于普通二维树状数组
// 注意，对象必须声明在全局
OY::GBIT2D::Tree<uint32_t, uint64_t, true, true, 1009> Gbit(100000, 100000);
int main() {
    Gbit.add(0, 100000, 0, 100000, 1);
    Gbit.add(100000, 500000, 100000, 500000, 2);
    cout << Gbit.query(50000, 450000, 50000, 450000) << endl;
    Gbit.clear();
    cout << Gbit.query(50000, 450000, 50000, 450000) << endl;
}
/*
#输出如下
2500000000
0

*/