#include "IO/FastIO.h"
#include "MATH/Euclidean.h"
#include "MATH/StaticModInt32.h"

int main() {
    uint32_t a = 12;
    uint32_t b = 7;
    uint32_t c = 25;
    uint32_t n = 10000;
    using mint = OY::mint1000000007;

    uint64_t F = 0, G = 0, H = 0;
    for (int i = 0; i <= n; i++) {
        F += (uint64_t(a) * i + b) / c;
        F %= mint::mod();
        G += (uint64_t(a) * i + b) / c * i;
        G %= mint::mod();
        H += (uint64_t(a) * i + b) / c * ((uint64_t(a) * i + b) / c);
        H %= mint::mod();
    }

    auto res = OY::Euclidean<mint>().calc(a, b, c, n);
    auto f = res.m_f, g = res.m_g, h = res.m_h;
    cout << F << "," << f << endl;
    cout << G << "," << g << endl;
    cout << H << "," << h << endl;
}
/*
#输出如下
24000400,24000400
13995080,13995080
801916668,801916668

*/