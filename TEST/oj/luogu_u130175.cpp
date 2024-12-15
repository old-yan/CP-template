#include "DS/ErasableDSU.h"
#include "IO/FastIO.h"

/*
[U130175 myf与前女友](https://www.luogu.com.cn/problem/U130175)
*/
/**
 * 移动操作其实就是移除之后再做一次连结
 */

void solve_edsu() {
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
            uint32_t a, b;
            cin >> a >> b, a--, b--;
            if (u.in_same_group(a, b)) continue;
            u.unite_to(u.extract(a), u.find(b));
        } else {
            uint32_t a, b;
            cin >> a >> b, a--, b--;
            cout << (u.in_same_group(a, b) ? "YES\n" : "NO\n");
        }
    }
    cout << u.count();
}

int main() {
    solve_edsu();
}