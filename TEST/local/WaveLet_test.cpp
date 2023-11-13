#include "DS/WaveLet.h"
#include "IO/FastIO.h"

int main() {
    // 先给出一个长度为 10 的区间
    int A[10] = {1, 5, 6, 3, 8, 4, 4, 2, 10, 1};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个默认小波表
    auto wt = OY::WaveLetTable<uint32_t, uint64_t, 1000>(A, A + 10);

    // 区间第 k
    cout << "A[3~6] No.1 = " << wt.quantile(3, 6, 0) << endl;
    cout << "A[3~6] No.2 = " << wt.quantile(3, 6, 1) << endl;
    cout << "A[3~6] No.3 = " << wt.quantile(3, 6, 2) << endl;
    cout << "A[3~6] No.4 = " << wt.quantile(3, 6, 3) << endl;

    // 区间值域统计个数
    cout << "A[0~6].count(1) = " << wt.count(0, 6, 1) << endl;
    cout << "A[0~6].count(4) = " << wt.count(0, 6, 4) << endl;
    cout << "A[0~6].count(1~4) = " << wt.count(0, 6, 1, 4) << endl;

    // 区间排名
    cout << "A[3~6] rank of (3) = " << wt.rank(3, 6, 3) << endl;
    cout << "A[3~6] rank of (4) = " << wt.rank(3, 6, 4) << endl;
    cout << "A[3~6] rank of (5) = " << wt.rank(3, 6, 5) << endl;
    cout << "A[3~6] rank of (6) = " << wt.rank(3, 6, 6) << endl;

    // 区间最小值
    cout << "A[3~6] minimum = " << wt.minimum(3, 6) << endl;

    // 区间最大值
    cout << "A[3~6] maximum = " << wt.maximum(3, 6) << endl;

    // 区间最大异或
    cout << "A[3~6].max_bitxor(9) = " << wt.max_bitxor(3, 6, 9) << endl;

    std::string B[] = {"hello", "app", "app", "world", "banana", "app", "banana", "hello"};
    // 建立一个默认小波树
    auto wt2 = OY::WaveLetTree<std::string, uint64_t, 1000>(B, B + 8);

    // 区间第 k
    cout << "B[1~6] No.1 = " << wt2.quantile(1, 6, 0) << endl;
    cout << "B[1~6] No.2 = " << wt2.quantile(1, 6, 1) << endl;
    cout << "B[1~6] No.3 = " << wt2.quantile(1, 6, 2) << endl;
    cout << "B[1~6] No.4 = " << wt2.quantile(1, 6, 3) << endl;
    cout << "B[1~6] No.5 = " << wt2.quantile(1, 6, 4) << endl;
    cout << "B[1~6] No.6 = " << wt2.quantile(1, 6, 5) << endl;

    // 区间值域统计个数
    cout << "B[1~6].count(\"app\") = " << wt2.count(1, 6, "app") << endl;
    cout << "B[1~6].count(\"yy\") = " << wt2.count(1, 6, "yy") << endl;
    cout << "B[1~6].count(\"apple\"~\"cat\") = " << wt2.count(1, 6, "apple", "cat") << endl;

    // 区间排名
    cout << "B[1~6] rank of (\"ap\")     = " << wt2.rank(1, 6, "ap") << endl;
    cout << "B[1~6] rank of (\"app\")    = " << wt2.rank(1, 6, "app") << endl;
    cout << "B[1~6] rank of (\"apple\")    = " << wt2.rank(1, 6, "apple") << endl;
    cout << "B[1~6] rank of (\"banana\") = " << wt2.rank(1, 6, "banana") << endl;

    // 区间最小值
    cout << "B[1~6] minimum = " << wt2.minimum(1, 6) << endl;

    // 区间最大值
    cout << "B[1~6] maximum = " << wt2.maximum(1, 6) << endl;
}
/*
#输出如下
1 5 6 3 8 4 4 2 10 1
A[3~6] No.1 = 3
A[3~6] No.2 = 4
A[3~6] No.3 = 4
A[3~6] No.4 = 8
A[0~6].count(1) = 1
A[0~6].count(4) = 2
A[0~6].count(1~4) = 4
A[3~6] rank of (3) = 0
A[3~6] rank of (4) = 1
A[3~6] rank of (5) = 3
A[3~6] rank of (6) = 3
A[3~6] minimum = 3
A[3~6] maximum = 8
A[3~6].max_bitxor(9) = 13
B[1~6] No.1 = app
B[1~6] No.2 = app
B[1~6] No.3 = app
B[1~6] No.4 = banana
B[1~6] No.5 = banana
B[1~6] No.6 = world
B[1~6].count("app") = 3
B[1~6].count("yy") = 0
B[1~6].count("apple"~"cat") = 2
B[1~6] rank of ("ap")     = 0
B[1~6] rank of ("app")    = 0
B[1~6] rank of ("apple")    = 3
B[1~6] rank of ("banana") = 3
B[1~6] minimum = app
B[1~6] maximum = world

*/