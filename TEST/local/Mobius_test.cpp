#include "IO/FastIO.h"
#include "MATH/Mobius.h"

#include <random>

OY::MOBIUS::Table<100000, true> Mu;
void test0() {
    // 对于有两个或两个以上相同质因子的数字，莫比乌斯函数为 0
    int s = 3 * 11 * 11 * 17;
    cout << "\\mu(3 * 11 * 11 * 17) = " << Mu.query_mobius(s) << endl;

    s = 17 * 17 * 17;
    cout << "\\mu(17 * 11 * 17) = " << Mu.query_mobius(s) << endl;
}

void test1() {
    // 对于没有两个或两个以上相同质因子的数字
    // 如果质因子种类为偶数，莫比乌斯函数为 1
    int s = 3 * 7 * 11 * 17;
    cout << "\\mu(3 * 7 * 11 * 17) = " << Mu.query_mobius(s) << endl;
    s = 1;
    cout << "\\mu(1) = " << Mu.query_mobius(s) << endl;

    // 如果质因子种类为奇数，莫比乌斯函数为 -1
    s = 3 * 11 * 17;
    cout << "\\mu(3 * 11 * 17) = " << Mu.query_mobius(s) << endl;
    s = 2 * 3 * 5 * 7 * 17;
    cout << "\\mu(2 * 3 * 5 * 7 * 17) = " << Mu.query_mobius(s) << endl
         << endl;
}

void test_range_mobius() {
    cout << "\\mu(95) = " << Mu.query_mobius(95) << endl;
    cout << "\\mu(96) = " << Mu.query_mobius(96) << endl;
    cout << "\\mu(97) = " << Mu.query_mobius(97) << endl;
    cout << "\\mu(98) = " << Mu.query_mobius(98) << endl;
    cout << "\\mu(99) = " << Mu.query_mobius(99) << endl;
    cout << "\\mu(100) = " << Mu.query_mobius(100) << endl;
    cout << "\\mu(101) = " << Mu.query_mobius(101) << endl;
    cout << "\\mu(102) = " << Mu.query_mobius(102) << endl;
    cout << "\\mu(95~102) = " << Mu.query_mobius(95, 102) << endl
         << endl;
}

void test_multipicative() {
    cout << "test of multiplicative calculator:\n";
    // 尝试计算 f(x)=x的不同因子的数量
    // 众所周知，一个数包含的不同因子的数量，为积性函数
    auto calc_prime = [](int p) {
        // 计算 f(p)
        // 此处 p 为质数
        return 2; // 显然，质数只有两个因子，1 和自身
    };
    auto calc_prime_pow = [&](int p, int cnt, int low) {
        // 计算 f(p^cnt)
        // 此处 p 为质数，cnt 为次数，low = p ^ (cnt-1)
        // 显然，f(p^cnt)=f(p^(cnt-1))+1
        // 或者 f(p^cnt)=cnt+1
        return cnt + 1;
    };
    auto f = OY::MOBIUS::Multiplicative<100>().solve<int>(100, calc_prime, calc_prime_pow);
    cout << "count of factors of 36 = " << f[36] << endl;
    cout << "count of factors of 49 = " << f[49] << endl
         << endl;
}

void test_Dirichlet_divisor() {
    cout << "test of Dirichlet_divisor:\n";
    // 利用迪利克雷前缀和，求每个数字的因子数
    int a[101]{};
    for (int i = 1; i <= 100; i++) a[i] = 1;
    OY::MOBIUS::partial_sum_Dirichlet_divisor(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of factors of 36 = " << a[36] << endl;
    cout << "count of factors of 49 = " << a[49] << endl;
         
    // 利用迪利克雷前缀和，求每个数字的因子的因子数之和
    OY::MOBIUS::partial_sum_Dirichlet_divisor(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of count of factors of 36 = " << a[36] << endl;
    cout << "count of count of factors of 49 = " << a[49] << endl
         << endl;
}

void test_Dirichlet_multiple() {
    cout << "test of Dirichlet_multiple:\n";
    // 利用迪利克雷前缀和，求每个数字的倍数数量
    int a[101]{};
    for (int i = 1; i <= 100; i++) a[i] = 1;
    OY::MOBIUS::partial_sum_Dirichlet_multiple(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of multiples of 12 = " << a[12] << endl;
    cout << "count of multiples of 16 = " << a[16] << endl;
         
    // 利用迪利克雷前缀和，求每个数字的倍数倍数数量之和
    OY::MOBIUS::partial_sum_Dirichlet_multiple(100, a, [&](uint32_t k) { return Mu.query_kth_prime(k); });
    cout << "count of count of multiples of 12 = " << a[12] << endl;
    cout << "count of count of multiples of 16 = " << a[16] << endl
         << endl;
}

void test_extra() {
    // 只有 1 的因子的莫比乌斯函数和为 1
    // 所以 [x==1] == \Sigma{d|x} \mu(d)
    for (int i = 1; i <= 10; i++) {
        int sum = 0;
        for (int divisor = 1; divisor <= i; divisor++)
            if (i % divisor == 0) {
                sum += Mu.query_mobius(divisor);
            }
        cout << "sum(\\mu(divisor of " << i << ")) = " << sum << endl;
    }
}

int main() {
    test0();
    test1();
    test_range_mobius();
    test_multipicative();
    test_Dirichlet_divisor();
    test_Dirichlet_multiple();
    test_extra();
}
/*
#输出如下
\mu(3 * 11 * 11 * 17) = 0
\mu(17 * 11 * 17) = 0
\mu(3 * 7 * 11 * 17) = 1
\mu(1) = 1
\mu(3 * 11 * 17) = -1
\mu(2 * 3 * 5 * 7 * 17) = -1

\mu(95) = 1
\mu(96) = 0
\mu(97) = -1
\mu(98) = 0
\mu(99) = 0
\mu(100) = 0
\mu(101) = -1
\mu(102) = -1
\mu(95~102) = -2

test of multiplicative calculator:
count of factors of 36 = 9
count of factors of 49 = 3

test of Dirichlet_divisor:
count of factors of 36 = 9
count of factors of 49 = 3
count of count of factors of 36 = 36
count of count of factors of 49 = 6

test of Dirichlet_multiple:
count of multiples of 12 = 8
count of multiples of 16 = 6
count of count of multiples of 12 = 20
count of count of multiples of 16 = 14

sum(\mu(divisor of 1)) = 1
sum(\mu(divisor of 2)) = 0
sum(\mu(divisor of 3)) = 0
sum(\mu(divisor of 4)) = 0
sum(\mu(divisor of 5)) = 0
sum(\mu(divisor of 6)) = 0
sum(\mu(divisor of 7)) = 0
sum(\mu(divisor of 8)) = 0
sum(\mu(divisor of 9)) = 0
sum(\mu(divisor of 10)) = 0

*/