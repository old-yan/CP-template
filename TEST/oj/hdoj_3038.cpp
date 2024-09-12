#include "DS/PotentializedDSU.h"
#include "IO/FastIO.h"

/*
[How Many Answers Are Wrong](https://acm.hdu.edu.cn/showproblem.php?pid=3038)
*/
/**
 * 维护势能关系的并查集
 * 或称权值并查集
 */

int main() {
    uint32_t n, m;
    while (cin >> n >> m) {
        OY::AddPDSUTable<uint32_t, true> U(n + 1);
        uint32_t ans{};
        while (m--) {
            uint32_t l, r, dis;
            cin >> l >> r >> dis;
            if (!U.unite_by_size(l - 1, r, dis) && U.calc(l - 1, r).second != dis) ans++;
        }
        cout << ans << endl;
    }
}