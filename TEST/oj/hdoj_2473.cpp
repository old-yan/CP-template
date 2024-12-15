#include "DS/ErasableDSU.h"
#include "IO/FastIO.h"

/*
[Junk-Mail Filter](https://acm.hdu.edu.cn/showproblem.php?pid=2473)
*/
/**
 * 带删除的并查集模板题
 */

void solve_edsu() {
    for (uint32_t t = 1;; t++) {
        uint32_t n, m;
        cin >> n >> m;
        if (!n) break;
        OY::EDSU::Table u(n);
        while (m--) {
            char op;
            cin >> op;
            if (op == 'M') {
                uint32_t a, b;
                cin >> a >> b;
                u.unite_by_size(a, b);
            } else {
                uint32_t x;
                cin >> x;
                u.extract(x);
            }
        }
        cout << "Case #" << t << ": " << u.count() << endl;
    }
}

int main() {
    solve_edsu();
}