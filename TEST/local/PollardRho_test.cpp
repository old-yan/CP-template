#include "IO/FastIO.h"
#include "MATH/PollardRho.h"

int main() {
    uint64_t A = 2ull * 3 * 3 * 5 * 5 * 95986273 * 265988969;
    auto pairs = OY::PollardRho::decomposite<true>(A);
    for (auto &&pair : pairs) {
        cout << pair.m_prime << '^' << pair.m_count << endl;
    }

    uint64_t B = 2 * 3 * 5 * 7;
    auto fs = OY::PollardRho::get_factors<true>(B);
    for (auto f : fs) {
        cout << B << " % " << f << " = " << B % f << endl;
    }
}
/*
#输出如下
2^1
3^2
5^2
95986273^1
265988969^1
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

*/