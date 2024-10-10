#include "DS/BIT_ex.h"
#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[#132. 树状数组 3 ：区间修改，区间查询](https://loj.ac/p/132)
*/
/**
 * 本题为树状数组模板题
 */

template <typename Table>
void solve() {
    uint32_t n, q;
    cin >> n >> q;
    auto read = [](auto...) -> int64_t {
        int64_t x;
        cin >> x;
        return x;
    };
    Table tree(n, read);
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            int64_t x;
            cin >> l >> r >> x;
            tree.add(l - 1, r - 1, x);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << tree.query(l - 1, r - 1) << endl;
        }
    }
}

int main() {
    solve<OY::StaticBIT_ex<int64_t, 1 << 20>>();
    // solve<OY::ZkwLazySumTree<int64_t>>();
    // solve<OY::VectorSegLazySumTree<int64_t, uint32_t>>();
}