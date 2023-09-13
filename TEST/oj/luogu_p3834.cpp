#include "DS/DivideTree.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[P3834 【模板】可持久化线段树 2](https://www.luogu.com.cn/problem/P3834)
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto S = OY::make_DivideTree<int, std::less<int>, 1 << 24>(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    // auto S = OY::WaveLetTree<uint32_t, uint64_t, 201000>(
    //     n, [](auto...) {
    //         uint32_t x;
    //         cin >> x;
    //         return x;
    //     });
    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r, k;
        cin >> l >> r >> k;
        cout << S.quantile(l - 1, r - 1, k - 1) << endl;
    }
}