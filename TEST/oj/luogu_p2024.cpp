#include "DS/PotentializedDSU.h"
#include "IO/FastIO.h"

/*
[P2024 [NOI2001] 食物链](https://www.luogu.com.cn/problem/P2024)
*/
/**
 * 本题常用的做法是种类并查集
 * 此处可以用维护势能关系的并查集
 */

int main() {
    uint32_t n, k;
    cin >> n >> k;
    OY::ModAddPDSUTable<uint32_t, 3, true> U(n);
    uint32_t ans{};
    while (k--) {
        char op;
        uint32_t x, y;
        cin >> op >> x >> y;
        if (x <= n && y <= n)
            if (op == '1')
                ans += !U.unite_by_size(x - 1, y - 1, 0) && U.calc(x - 1, y - 1).second;
            else
                ans += !U.unite_by_size(y - 1, x - 1, 1) && U.calc(y - 1, x - 1).second != 1;
        else
            ans++;
    }
    cout << ans;
}