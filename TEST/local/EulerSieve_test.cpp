#include "IO/FastIO.h"
#include "MATH/EulerSieve.h"

int main() {
    OY::EulerSieve::Sieve<10000000, true, true, true> ps;
    cout << "number of primes in [0, 10000000]: " << ps.count() << endl;
    cout << "No.0 prime is: " << ps.query_kth_prime(0) << endl;
    cout << "No.1 prime is: " << ps.query_kth_prime(1) << endl;
    cout << "No.2 prime is: " << ps.query_kth_prime(2) << endl;
    cout << "No.3 prime is: " << ps.query_kth_prime(3) << endl;
    cout << "No.4 prime is: " << ps.query_kth_prime(4) << endl;
    cout << "No.500000 prime is: " << ps.query_kth_prime(500000) << endl;

    cout << "1234567 is prime?" << (ps.is_prime(1234567) ? "yes\n" : "no\n");
    cout << "1234571 is prime?" << (ps.is_prime(1234571) ? "yes\n" : "no\n");

    cout << "smallest prime factor of 30: " << ps.query_smallest_factor(30) << endl;
    cout << "biggest prime factor of 30: " << ps.query_biggest_factor(30) << endl;

    uint32_t A = 2 * 3 * 3 * 5 * 5 * 97 * 101;
    auto pf = ps.decomposite(A);
    for (auto [p, c] : pf) {
        cout << p << '^' << c << endl;
    }

    uint32_t B = 2 * 3 * 5 * 7;
    auto fs = ps.get_factors<true>(B);
    for (auto f : fs) {
        cout << B << " % " << f << " = " << B % f << endl;
    }

    // 如果不想进行各种麻烦操作，只想知道质数有几个，可以把模板参数设为 false
    OY::EulerSieve::Sieve<10000000> simple_ps;
    cout << "number of primes in [0, 10000000]: " << simple_ps.count() << endl;
}
/*
#输出如下
number of primes in [0, 10000000]: 664579
No.0 prime is: 2
No.1 prime is: 3
No.2 prime is: 5
No.3 prime is: 7
No.4 prime is: 11
No.500000 prime is: 7368791
1234567 is prime?no
1234571 is prime?no
smallest prime factor of 30: 2
biggest prime factor of 30: 5
2^1
3^2
5^2
97^1
101^1
210 % 1 = 0
210 % 2 = 0
210 % 3 = 0
210 % 5 = 0
210 % 6 = 0
210 % 7 = 0
210 % 10 = 0
210 % 14 = 0
210 % 15 = 0
210 % 21 = 0
210 % 30 = 0
210 % 35 = 0
210 % 42 = 0
210 % 70 = 0
210 % 105 = 0
210 % 210 = 0
number of primes in [0, 10000000]: 664579

*/