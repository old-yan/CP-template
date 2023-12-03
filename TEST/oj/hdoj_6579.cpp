#include "IO/FastIO.h"
#include "MATH/HamelXorBaseTable.h"

/*
[Operation](https://acm.hdu.edu.cn/showproblem.php?pid=6579)
*/
/**
 * 本题为前缀线性基模板题
 */

int main() {
    uint32_t t;
    cin >> t;
    OY::StaticHamelXorBaseTable32<30> S;
    while (t--) {
        uint32_t n, m;
        cin >> n >> m;
        S.reserve(n + m);
        for (uint32_t i = 0; i < n; i++) {
            uint32_t x;
            cin >> x;
            S.push_back(x);
        };
        uint32_t lst = 0;
        for (uint32_t i = 0; i < m; i++) {
            char op;
            cin >> op;
            if (op == '0') {
                uint32_t l, r;
                cin >> l >> r;
                l = (l ^ lst) % n, r = (r ^ lst) % n;
                if (l > r) std::swap(l, r);
                lst = S.query_max_bitxor(l, r);
                cout << lst << endl;
            } else {
                uint32_t x;
                cin >> x;
                S.push_back(x ^ lst);
                n++;
            }
        }
    }
}