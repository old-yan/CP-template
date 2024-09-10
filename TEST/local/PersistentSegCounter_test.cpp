#include "DS/PersistentSegCounter.h"
#include "IO/FastIO.h"

void test() {
    // 这是一个长度为5的数组
    int A[5] = {100, 200, 300, 400, 500};
    using PerCounter = OY::PerSEGCNT::Table<uint32_t, int, true, false, false>;

    std::vector<PerCounter> counters;
    // counters[0] 作为基础
    counters.push_back({});
    for (int i = 0; i < 5; i++) {
        counters.push_back(counters.back().copy());
        counters.back().add(A[i], 1);
    }

    // 两个 counter 可以做差，得到一个差视图
    cout << "diff of counters[1] and counters[4]" << counters[4] - counters[1] << endl;

    // 输出 [200,300,400] 里和 150 进行异或的最大异或结果
    cout << "max_bitxor with 150: " << (counters[4] - counters[1]).max_bitxor(150) << endl;

    // 输出 [200,300,400] 里和 150 进行异或的最小异或结果
    cout << "min_bitxor with 150: " << (counters[4] - counters[1]).min_bitxor(150) << endl;

    // 输出 [200,300,400] 里的最大键
    cout << "max: " << (counters[4] - counters[1]).max() << endl;

    // 输出 [200,300,400] 里的最小键
    cout << "min: " << (counters[4] - counters[1]).min() << endl;

    // 输出 [200,300,400] 里，键位于 [200,300] 的个数
    cout << "count(200~300) = " << (counters[4] - counters[1]).query(200, 300) << endl;

    // 输出 [200,300,400] 里，和 150 异或后，位于 [200,300] 的个数
    cout << "count([200<=x^150<=300]) = " << (counters[4] - counters[1]).query_bitxor(200, 300, 150) << endl;
}

int main() {
    test();
}
/*
#输出如下
diff of counters[1] and counters[4]{200*1,300*1,400*1}
max_bitxor with 150: 442
min_bitxor with 150: 94
max: 400
min: 200
count(200~300) = 2
count([200<=x^150<=300]) = 1

*/