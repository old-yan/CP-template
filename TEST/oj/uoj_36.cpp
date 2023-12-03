#include "IO/FastIO.h"
#include "MATH/HamelXorBase.h"

/*
[#36. 【清华集训2014】玛里苟斯](https://uoj.ac/problem/36)
*/
/**
 * 本题需要分情况讨论
 * 当 k == 1 时，每位独立
 * 当 k == 2 时，根据每两位之间的独立关系确定最终贡献
 * 当 k > 2 时，枚举所有可能的掩膜
 */

static constexpr uint32_t N = 100000;
int main() {
    uint64_t n, K;
    cin >> n >> K;

    if (K == 1) {
        uint64_t sum = 0;
        for (uint32_t i = 0; i < n; i++) {
            uint64_t x;
            cin >> x;
            sum |= x;
        }
        cout << sum / 2;
        if (sum & 1) cout << ".5";
    } else if (K == 2) {
        OY::StaticHamelXorBase32<32> hxb{};
        uint32_t sum = 0;
        for (uint32_t i = 0; i < n; i++) {
            uint32_t x;
            cin >> x;
            hxb.insert(x);
            sum |= x;
        }
        uint64_t ans = 0;
        for (uint32_t j1 = 0; j1 < 32; j1++)
            if (sum >> j1 & 1)
                for (uint32_t j2 = 0; j2 < 32; j2++)
                    if (sum >> j2 & 1)
                        if (hxb.is_bonded(j1, j2))
                            ans += uint64_t(1) << (j1 + j2);
                        else
                            ans += uint64_t(1) << (j1 + j2 - 1);
        cout << ans / 2;
        if (ans & 1) cout << ".5";
    } else {
        OY::StaticHamelXorBase32<21> hxb{};
        for (uint32_t i = 0; i < n; i++) {
            uint32_t x;
            cin >> x;
            hxb.insert(x);
        }
        uint32_t tot = hxb.total_count();
        __uint128_t ans = 0;
        hxb.enumerate([&](uint32_t mask) {
            __uint128_t prod = 1;
            for (uint32_t j = 0; j < K; j++) prod *= mask;
            ans += prod;
        });
        cout << uint64_t(ans / tot);
        if (ans % tot) cout << ".5";
    }
}