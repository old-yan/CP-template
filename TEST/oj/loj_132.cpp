#include "DS/BIT.h"
#include "IO/FastIO.h"

/*
[#132. 树状数组 3 ：区间修改，区间查询](https://loj.ac/p/132)
*/
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::BIT64<1> tree(n, [](auto...) -> int64_t {
        int64_t x;
        cin >> x;
        return x;
    });
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