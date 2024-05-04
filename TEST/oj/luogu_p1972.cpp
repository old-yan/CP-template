#include "DS/BIT.h"
#include "DS/KindCounter.h"
#include "DS/OfflineKindCounter.h"
#include "DS/WTree.h"
#include "IO/FastIO.h"

/*
[P1972 [SDOI2009] HH的项链](https://www.luogu.com.cn/problem/P1972)
*/
/**
 * 将每个元素的上次出现的位置列出来
 * 区间查询种类数，就是查询上次出现在区间左侧的元素数量
 * 可以使用小波树封装成的区间种类查询器
 *
 * 此外，还有个离线处理贡献的经典做法
 */

uint32_t val[1000000];
void solve_bit() {
    uint32_t n;
    cin >> n;
    for (uint32_t i = 0; i != n; i++) cin >> val[i];
    uint32_t m;
    cin >> m;
    auto query_provider = [](uint32_t, auto consumer) {
        uint32_t l, r;
        cin >> l >> r;
        consumer(l - 1, r - 1);
    };
    auto mapping = [](uint32_t i) { return val[i]; };
    auto ans = OY::OFFLINEKC::Solve<OY::WTree::Tree<uint32_t>, OY::OFFLINEKC::ArrayTag<1000000>>(m, query_provider, n, mapping);
    for (uint32_t i = 0; i < m; i++) cout << ans[i] << endl;
}

void solve_wavelet() {
    uint32_t n;
    cin >> n;
    OY::ArrayKindCounter<1000000> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    uint32_t m;
    cin >> m;
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << endl;
    }
}

int main() {
    solve_bit();
    // solve_wavelet();
}