#include "DS/PersistentBIT.h"
#include "IO/FastIO.h"

void test() {
    // 这是一个长度为5的数组
    int A[5] = {10000, 20000, 30000, 40000, 50000};
    // 写一个默认的支持区间加的求和树
    OY::PerBIT::Tree<int64_t, uint32_t, OY::PerBIT::VectorTag, true, true> T(A, A + 5);
    cout << "ver 0:" << T.get_version_view(0) << endl;

    T.copy_as(100);
    cout << "ver 100:" << T.get_version_view(100) << endl;
    // 对最后一个版本进行修改
    T.add(0, 66);
    cout << "ver 100:" << T.get_version_view(100) << endl;

    T.copy_as(200);
    T.add(1, 77);
    T.copy_as(300);
    T.add(2, 88);
    T.copy_as(400);
    T.add(3, 99);
    cout << "ver 400:" << T.get_version_view(400) << endl;

    // 可以回滚一个版本 （把版本 400 彻底删除掉）
    T.rollback();
    T.copy_as(500);
    T.add(3, 4, 101);
    cout << "ver 300:" << T.get_version_view(300) << endl;
    cout << "ver 500:" << T.get_version_view(500) << endl;

    // 在版本之间的差分树上做 kth 查询
    cout << "0-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 0) << endl;
    cout << "1-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 1) << endl;
    cout << "100-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 100) << endl;
    cout << "101-th elem in ver(300, 500]:" << T.reduce_kth(300, 500, 101) << endl;
}

int main() {
    test();
}
/*
#输出如下
ver 0:[10000, 20000, 30000, 40000, 50000]
ver 100:[10000, 20000, 30000, 40000, 50000]
ver 100:[10066, 20000, 30000, 40000, 50000]
ver 400:[10066, 20077, 30088, 40099, 50000]
ver 300:[10066, 20077, 30088, 40000, 50000]
ver 500:[10066, 20077, 30088, 40101, 50101]
0-th elem in ver(300, 500]:3
1-th elem in ver(300, 500]:3
100-th elem in ver(300, 500]:3
101-th elem in ver(300, 500]:4

*/