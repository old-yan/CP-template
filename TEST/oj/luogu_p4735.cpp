#include "DS/PersistentBiTrie.h"
#include "IO/FastIO.h"

/*
[P4735 最大异或和](https://www.luogu.com.cn/problem/P4735)
*/
/**
 * 本题显然可以用两棵 01 字典树做差，实现区间查询
 */
static constexpr uint32_t N = 300000, M = 300000, L = 24;
struct Info {
    uint32_t cnt;
};
using Tree = OY::PerBiTrie32<L, Info, false, (N + M) * L + 1>;
using iterator = Tree::iterator;
uint32_t A[N + M + 1], ans[M];

Tree pool[N + M + 1];
int main() {
    uint32_t n, m;
    cin >> n >> m;
    pool[0].init();
    pool[0].insert(0);
    for (uint32_t i = 1; i <= n; i++) {
        cin >> A[i];
        A[i] ^= A[i - 1];
        (pool[i] = pool[i - 1].copy()).insert(A[i], [](iterator it) { it->cnt++; });
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
            A[len] = A[len - 1] ^ x;
            (pool[len] = pool[len - 1].copy()).insert(A[len], [](iterator it) { it->cnt++; });
        } else {
            uint32_t l, r, x;
            cin >> l >> r >> x;
            x ^= A[len];
            uint32_t res;
            if (l > 1)
                res = Tree::reduce_max_bitxor(pool[l - 2], pool[r - 1], x, [](iterator it1, iterator it2) {
                    return it2->cnt > it1->cnt;
                });
            else
                res = pool[r - 1].query_max_bitxor(x).second;
            cout << res << endl;
        }
    }
}