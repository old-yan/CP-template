#include "DS/BIT.h"
#include "IO/FastIO.h"

/*
[#130. 树状数组 1 ：单点修改，区间查询](https://loj.ac/p/130)
*/
int main() {
    uint32_t n, q;
    cin >> n >> q;
    OY::BIT64<false, 1 << 20> tree(n, [](auto...) {
        int64_t x;
        cin >> x;
        return x;
    });
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t i;
            int64_t x;
            cin >> i >> x;
            tree.add(i - 1, x);
        } else {
            uint32_t l, r;
            cin >> l >> r;
            cout << tree.query(l - 1, r - 1) << endl;
        }
    }
}