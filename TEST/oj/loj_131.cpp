#include "DS/BIT.h"
#include "IO/FastIO.h"

/*
[#131. 树状数组 2 ：区间修改，单点查询](https://loj.ac/p/131)
*/
int main() {
    uint32_t n, q;
    cin >> n >> q;
    int64_t last = 0;
    OY::BIT64<false, 1 << 20> tree(n + 1, [&](uint32_t i) -> int64_t {
        if (i == n) return 0;
        int64_t x = last;
        cin >> last;
        return last - x;
    });
    while (q--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t l, r;
            int64_t x;
            cin >> l >> r >> x;
            tree.add(l - 1, x);
            tree.add(r, -x);
        } else {
            uint32_t i;
            cin >> i;
            cout << tree.presum(i - 1) << endl;
        }
    }
}