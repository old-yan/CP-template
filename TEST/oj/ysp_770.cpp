#include "DS/OfflineKindCounter.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[Static Range Count Distinct](https://judge.yosupo.jp/problem/static_range_count_distinct)(https://github.com/yosupo06/library-checker-problems/issues/770)
*/
/**
 * 本题可以作为小波树模板题，用小波树在线回答区间种类数
 * 当然跑得最快的还得离线查询
 */

static constexpr uint32_t N = 500000;
uint32_t arr[N];
int main() {
    uint32_t n, q;
    cin >> n >> q;
    if (!n) {
        for (uint32_t i = 0; i != q; i++) cout << "0\n";
        return 0;
    }
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::OfflineHashmapKindCounter<false, 700001> kc(n, read, q);
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r;
        cin >> l >> r;
        kc.add_query(l, r - 1);
    }
    for (auto a : kc.solve<OY::WSumTree<uint32_t>>()) cout << a << endl;
}
