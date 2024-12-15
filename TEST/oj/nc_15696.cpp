#include "DS/ErasableDSU.h"
#include "IO/FastIO.h"

/*
[Professional Manager](https://ac.nowcoder.com/acm/problem/15696)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/106/C
 * 带删除的并查集模板
 */

void solve_edsu() {
    uint32_t t;
    cin >> t;
    for (uint32_t i = 1; i <= t; i++) {
        cout << "Case #" << i << ":\n";
        uint32_t n, q;
        cin >> n >> q;
        OY::EDSU::Table u(n);
        while (q--) {
            char op;
            cin >> op;
            if (op == '1') {
                uint32_t a, b;
                cin >> a >> b, a--, b--;
                u.unite_by_size(a, b);
            } else if (op == '2') {
                uint32_t a;
                cin >> a, a--;
                u.extract(a);
            } else if (op == '3') {
                uint32_t a;
                cin >> a, a--;
                cout << u.size(a) << endl;
            } else {
                uint32_t a, b;
                cin >> a >> b, a--, b--;
                cout << (u.in_same_group(a, b) ? "YES\n" : "NO\n");
            }
        }
    }
}

int main() {
    solve_edsu();
}