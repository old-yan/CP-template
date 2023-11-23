#include "IO/FastIO.h"
#include "MATH/Euclidean.h"
#include "MATH/StaticModInt32.h"

int main() {
    uint32_t a = 12;
    uint32_t b = 7;
    uint32_t c = 25;
    uint32_t n = 10000;
    static constexpr uint32_t P = 1000000007;
    using mint = OY::StaticModInt32<P, true>;

    uint64_t F = 0, G = 0, H = 0;
    for (int i = 0; i <= n; i++) {
        F += (uint64_t(a) * i + b) / c;
        F %= P;
        G += (uint64_t(a) * i + b) / c * i;
        G %= P;
        H += (uint64_t(a) * i + b) / c * ((uint64_t(a) * i + b) / c);
        H %= P;
    }

    auto [f, g, h] = OY::Euclidean<mint>().calc(a, b, c, n);
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