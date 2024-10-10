#include "DS/WaveLet.h"
#include "IO/FastIO.h"

void test() {
    // 先给出一个长度为 10 的区间
    int A[10] = {1, 5, 6, 3, 8, 4, 4, 2, 10, 1};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个默认小波表
    auto wt = OY::WaveLet::Table<uint32_t>(A, A + 10);

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

    std::string B[] = {"hello", "app", "app", "world", "banana", "app", "banana", "hello"};
    // 建立一个默认小波树
    auto wt2 = OY::WaveLet::Tree<std::string>(B, B + 8);

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
    cout << "B[1~6] maximum = " << wt2.maximum(1, 6) << endl
         << endl;
}

void test_bitxor() {
    // 先给出一个长度为 10 的区间
    int A[10] = {31, 55, 38, 57, 60, 62, 35, 48, 36, 50};
    for (int i = 0; i < 10; i++) cout << A[i] << (i == 9 ? '\n' : ' ');

    // 建立一个默认小波表
    auto wt = OY::WaveLet::Table<uint32_t>(A, A + 10);

    // 区间 [2,7] 内，和 21 进行异或的第 k 异或（第 0 为最小）
    for (int i = 0; i < 6; i++) {
        auto res = wt.quantile_bitxor(2, 7, 21, i);
        int num = res ^ 21;
        cout << "A[2~7] No." << i + 1 << " : " << num << " ^ 21 = " << res << endl;
    }

    // 区间 [2,7] 内，和 21 进行异或得到的值里，和 [35, 40] 有没有交集
    if (wt.any_bitxor(2, 7, 21, 35, 40)) {
        cout << "[35,40] contains some result of A[2~7] xor 21\n";
    }

    // 区间 [2,7] 内，和 21 进行异或得到的值里，和 [45, 50] 有没有交集
    if (!wt.any_bitxor(2, 7, 21, 45, 50)) {
        cout << "[45,50] doesn't contain any result of A[2~7] xor 21\n";
    }
    
    cout << endl;
}

#include "DS/AdjDiff.h"
void test_k_sum() {
    // 借助差分模板，统计区间 k 大数的和
    int a[] = {40, 90, 20, 30, 50, 70, 80, 10, 60};
    cout << "\narr a:";
    for (int i = 0; i < 9; i++) cout << a[i] << " \n"[i == 8];

    using SumTable = OY::AdjSumTable<int, true>;
    OY::WaveLet::Table<uint32_t, SumTable> S(a, a + 9);

    // 统计 a[3~7] 最小的两个数的和
    auto f1 = [&](int l, int r, int k) {
        int res = 0;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, 0, k - 1, [&](int val) {
            res += val;
        });
        return res;
    };
    cout << "bottom-2 sum in a[3~7] = " << f1(3, 7, 2) << endl;

    // 统计 a[3~7] 最大的两个数的和
    auto f2 = [&](int l, int r, int k) {
        int res = 0;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, r - l + 1 - k, r - l, [&](int val) {
            res += val;
        });
        return res;
    };
    cout << "top-2 sum in a[3~7] = " << f2(3, 7, 2) << endl;
}

struct Multiplier {
    std::vector<int64_t> m_data;
    template <typename Iterator>
    void reset(Iterator first, Iterator last) {
        m_data.assign(first, last);
    }
    template <typename InitMapping>
    void resize(uint32_t length, InitMapping mapping) {
        m_data.resize(length);
        for (uint32_t i = 0; i != length; i++) m_data[i] = mapping(i);
    }
    int64_t query(int left, int right) const {
        int64_t res = 1;
        for (int i = left; i <= right; i++) res *= m_data[i];
        return res;
    }
};
void test_k_prod() {
    // 写一个简单的例子，表示如何实现区间 k 大数的乘积，或者其他的累积
    // 只需要实现 reset 和 query 两个方法
    // 注意，如果模板为 WaveLet::Tree，则只需要实现 resize 和 query 两个方法
    int a[] = {40, 90, 20, 30, 50, 70, 80, 10, 60};
    cout << "\narr a:";
    for (int i = 0; i < 9; i++) cout << a[i] << " \n"[i == 8];
    OY::WaveLet::Table<uint32_t, Multiplier> S(a, a + 9);
    // 统计 a[3~7] 最小的两个数的乘积
    auto f1 = [&](int l, int r, int k) {
        int64_t res = 1;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, 0, k - 1, [&](int64_t val) {
            res *= val;
        });
        return res;
    };
    cout << "bottom-2 prod in a[3~7] = " << f1(3, 7, 2) << endl;

    // 统计 a[3~7] 最大的两个数的乘积
    auto f2 = [&](int l, int r, int k) {
        int64_t res = 1;
        // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
        S.do_for_rank_range(l, r, r - l + 1 - k, r - l, [&](int64_t val) {
            res *= val;
        });
        return res;
    };
    cout << "top-2 prod in a[3~7] = " << f2(3, 7, 2) << endl;
}

void test_value_range_sum() {
    // 借助差分模板，统计区间 k 大数的和
    int a[] = {40, 90, 20, 30, 50, 70, 80, 10, 60};
    cout << "\narr a:";
    for (int i = 0; i < 9; i++) cout << a[i] << " \n"[i == 8];

    using SumTable = OY::AdjSumTable<int, true>;
    OY::WaveLet::Table<uint32_t, SumTable> S(a, a + 9);

    // 统计 a[1~7] 里，值在 [10, 60] 的数的和
    int res = 0;
    // 要传递一个回调函数，因为需要从好几个子区间上拼凑出最终结果
    S.do_for_value_range(1, 7, 10, 60, [&](int val) {
        res += val;
    });
    cout << "sum of elem([10, 60]) in a[1~7] = " << res << endl;
}

int main() {
    test();
    test_bitxor();
    test_k_sum();
    test_k_prod();
    test_value_range_sum();
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

31 55 38 57 60 62 35 48 36 50
A[2~7] No.1 : 48 ^ 21 = 37
A[2~7] No.2 : 60 ^ 21 = 41
A[2~7] No.3 : 62 ^ 21 = 43
A[2~7] No.4 : 57 ^ 21 = 44
A[2~7] No.5 : 38 ^ 21 = 51
A[2~7] No.6 : 35 ^ 21 = 54
[35,40] contains some result of A[2~7] xor 21
[45,50] doesn't contain any result of A[2~7] xor 21


arr a:40 90 20 30 50 70 80 10 60
bottom-2 sum in a[3~7] = 40
top-2 sum in a[3~7] = 150

arr a:40 90 20 30 50 70 80 10 60
bottom-2 prod in a[3~7] = 300
top-2 prod in a[3~7] = 5600

arr a:40 90 20 30 50 70 80 10 60
sum of elem([10, 60]) in a[1~7] = 110

*/