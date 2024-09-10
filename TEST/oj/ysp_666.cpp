#include "DS/GlobalRangeCounter.h"
#include "IO/FastIO.h"

/*
[Static Range Frequency](https://judge.yosupo.jp/problem/static_range_frequency)(https://github.com/yosupo06/library-checker-problems/issues/666)
*/
/**
 * 本题可以作为小波树模板题
 * 当然跑得最快的还得是把一个元素的所有下标收集起来再二分
 */

static constexpr uint32_t N = 500000;
OY::GlobalRangeCounter<uint32_t, 1 << 20> GRC;
int main() {
    uint32_t n, q;
    cin >> n >> q;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    GRC.resize(n, read);
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r, x;
        cin >> l >> r >> x;
        cout << GRC.query(l, r - 1, x) << endl;
    }
}