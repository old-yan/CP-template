#include "DS/PersistentBiTrie.h"
#include "IO/FastIO.h"

/*
[P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)
*/
/**
 * 本题显然可以用两棵 01 字典树做差，实现区间查询
 */

static constexpr uint32_t N = 300000, M = 300000, L = 24;
using Tree = OY::StaticCountPerBiTrie32<L, (N + M) * L + 1>;
using node = Tree::node;
uint32_t ans[M];
Tree pool[N + M + 1];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    pool[0].insert_one(0);
    uint32_t xorsum{};
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t x;
        cin >> x;
        xorsum ^= x;
        (pool[i] = pool[i - 1].copy()).insert_one(xorsum);
    }
    uint32_t len = n;
    uint32_t j = 0;
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == 'A') {
            uint32_t x;
            cin >> x;
            len++;
            xorsum ^= x;
            (pool[len] = pool[len - 1].copy()).insert_one(xorsum);
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            x ^= xorsum;
            uint32_t res;
            if (l == 1)
                res = pool[r - 1].max_bitxor(x).second;
            else
                res = (pool[r - 1] - pool[l - 2]).max_bitxor(x);
            cout << res << endl;
        }
    }
}