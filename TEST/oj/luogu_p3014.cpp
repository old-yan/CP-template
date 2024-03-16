#include "IO/FastIO.h"
#include "MATH/CantorRank.h"
#include "MATH/CantorSequence.h"
#include "MATH/StaticModInt64.h"

/*
[P3014 [USACO11FEB] Cow Line S](https://www.luogu.com.cn/problem/P3014)
*/
/**
 * 本题要求康托展开和逆康托展开
 */

int main() {
    using mint = OY::mint9223372036854775783;
    uint32_t n, k;
    cin >> n >> k;
    for (uint32_t i = 0; i < k; i++) {
        char op;
        cin >> op;
        if (op == 'P') {
            uint64_t rnk;
            cin >> rnk;
            auto seq = OY::get_Cantor_sequence(n, rnk - 1);
            for (auto &a : seq) cout << a + 1 << ' ';
            cout << endl;
        } else {
            std::vector<uint32_t> seq(n);
            for (auto &a : seq) cin >> a;
            cout << OY::CantorRank<mint>::raw_query(seq.begin(), seq.end()).val() + 1 << endl;
        }
    }
}