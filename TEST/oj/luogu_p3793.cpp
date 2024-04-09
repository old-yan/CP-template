#include "DS/MaskRMQ.h"
#include "DS/SqrtTree.h"
#include "IO/FastIO.h"

/*
[P3793 由乃救爷爷](https://www.luogu.com.cn/problem/P3793)
*/
/**
 * 本题为 RMQ 模板题
 * 可以使用 SqrtTree 或者 MaskRMQ
 */
namespace GenHelper {
    unsigned z1, z2, z3, z4, b;
    unsigned rand_() {
        b = ((z1 << 6) ^ z1) >> 13;
        z1 = ((z1 & 4294967294U) << 18) ^ b;
        b = ((z2 << 2) ^ z2) >> 27;
        z2 = ((z2 & 4294967288U) << 2) ^ b;
        b = ((z3 << 13) ^ z3) >> 21;
        z3 = ((z3 & 4294967280U) << 7) ^ b;
        b = ((z4 << 3) ^ z4) >> 12;
        z4 = ((z4 & 4294967168U) << 13) ^ b;
        return (z1 ^ z2 ^ z3 ^ z4);
    }
}
void srand(unsigned x) {
    using namespace GenHelper;
    z1 = x;
    z2 = (~x) ^ 0x233333333U;
    z3 = x ^ 0x1234598766U;
    z4 = (~x) + 51;
}
uint32_t read() {
    using namespace GenHelper;
    uint32_t a = rand_() & 32767;
    uint32_t b = rand_() & 32767;
    return a * 32768 + b;
}

int main() {
    uint32_t n, m, s;
    cin >> n >> m >> s;
    srand(s);
    auto S = OY::SqrtMaxTable<uint32_t, 20000000>(n, [](auto...) { return read(); });
    // auto S = OY::MaskRMQMaxValueTable<uint32_t, uint64_t, 20000000>(n, [](auto...) { return read(); });
    uint64_t sum = 0;
    for (uint32_t i = 0; i < m; i++) {
        auto l = read() % n, r = read() % n;
        if (l > r) std::swap(l, r);
        sum += S.query(l, r);
    }
    cout << sum;
}