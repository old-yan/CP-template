#include "IO/FastIO.h"
#include "MATH/FastSieve.h"

int main() {
    // 筛法找质数
    OY::FASTSIEVE::Sieve<100000000> ps;
    cout << "number of primes in [0, 100000000]: " << ps.count() << endl;
    cout << "No.0 prime is: " << ps.query_kth_prime(0) << endl;
    cout << "No.1 prime is: " << ps.query_kth_prime(1) << endl;
    cout << "No.2 prime is: " << ps.query_kth_prime(2) << endl;
    cout << "No.3 prime is: " << ps.query_kth_prime(3) << endl;
    cout << "No.4 prime is: " << ps.query_kth_prime(4) << endl;
    cout << "No.5000000 prime is: " << ps.query_kth_prime(5000000) << endl;

    // 但是这样很难查询某个数字是否为质数
    // 可以考虑把所有数字是否为质数的信息转到 bitset 里
    auto B = ps.to_bitset();
    cout << "1234567 is prime?" << (B[1234567] ? "yes" : "no") << endl;
    cout << "123457 is prime?" << (B[123457] ? "yes" : "no") << endl;
}
/*
#输出如下
number of primes in [0, 100000000]: 5761455
No.0 prime is: 2
No.1 prime is: 3
No.2 prime is: 5
No.3 prime is: 7
No.4 prime is: 11
No.5000000 prime is: 86028157
1234567 is prime?no
123457 is prime?yes

*/