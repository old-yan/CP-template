#include "DS/CatTree.h"
#include "DS/STTable.h"
#include "DS/SegTree.h"
#include "DS/SqrtTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P1890 gcd区间](https://www.luogu.com.cn/problem/P1890)
*/
/**
 * 显然，本题随便一个区间查询的数据结构就可以解决
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto S = OY::make_STTable<uint32_t, 1 << 14>(n, std::gcd<uint32_t>, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    // auto S = OY::make_CatTree<uint32_t, 1 << 14>(n, std::gcd<uint32_t>, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = OY::make_ZkwTree<uint32_t, 1 << 12>(n, std::gcd<uint32_t>, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = OY::make_SegTree<uint32_t, true, OY::Seg::Ignore, 1 << 12, uint32_t>(n, std::gcd<uint32_t>, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = OY::make_SqrtTree<uint32_t, 1 << 11>(n, std::gcd<uint32_t>, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << endl;
    }
}