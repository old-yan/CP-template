#include "DS/CatTree.h"
#include "DS/MonoZkwTree.h"
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
    auto my_gcd = [](uint32_t x, uint32_t y) { return std::gcd(x, y); };
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto S = OY::make_STTable<uint32_t, 14>(n, my_gcd, read);
    // auto S = OY::make_CatTree<uint32_t, 14>(n, my_gcd, read);
    // auto S = OY::MonoGcdTree<uint32_t>(n, read);
    // auto S = OY::make_ZkwTree<uint32_t>(n, my_gcd, read);
    // auto S = OY::make_SegTree<uint32_t, true, OY::Seg::Ignore, uint32_t>(n, my_gcd, read);
    // auto S = OY::make_SqrtTree<uint32_t, OY::Sqrt::RandomController<>, 8>(n, my_gcd, read);
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << endl;
    }
}