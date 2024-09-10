#include "DS/CatTree.h"
#include "DS/MonoZkwTree.h"
#include "DS/SparseTable.h"
#include "DS/SqrtTree.h"
#include "IO/FastIO.h"

/*
[P1890 gcd区间](https://www.luogu.com.cn/problem/P1890)
*/
/**
 * 显然，本题随便一个区间查询的数据结构就可以解决
 */

static constexpr uint32_t N = 1000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto my_gcd = [](uint32_t x, uint32_t y) { return std::gcd(x, y); };
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    auto S = OY::STGcdTable<uint32_t, 14>(n, read);
    // auto S = OY::CatGcdTable<uint32_t, 14>(n, read);
    // auto S = OY::MonoGcdTree<uint32_t>(n, read);
    // auto S = OY::make_SqrtTree<uint32_t, OY::SQRT::RandomController<>, 8>(n, my_gcd, read);
    while (m--) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << endl;
    }
}