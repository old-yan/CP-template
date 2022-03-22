#include "IO/FastIO.h"
#include "MATH/StaticEuclidean.h"

int main() {
    uint32_t a = 12;
    uint32_t b = 7;
    uint32_t c = 25;
    uint32_t n = 10000;
    constexpr uint32_t P = 1000000007;

    uint64_t F = 0, G = 0, H = 0;
    for (int i = 0; i <= n; i++) {
        F += (uint64_t(a) * i + b) / c;
        F %= P;
        G += (uint64_t(a) * i + b) / c * i;
        G %= P;
        H += (uint64_t(a) * i + b) / c * ((uint64_t(a) * i + b) / c);
        H %= P;
    }

    auto [f, g, h] = OY::StaticEuclidean<P>::calc(a, b, c, n);
    cout << F << "," << f << endl;
    cout << G << "," << g << endl;
    cout << H << "," << h << endl;
}