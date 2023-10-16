#include "DS/CatTree2D.h"
#include "DS/STTable2D.h"
#include "DS/ZkwTree2D.h"
#include "IO/FastIO.h"

/*
[Check Corners](http://acm.hdu.edu.cn/showproblem.php?pid=2888)
*/
/**
 * 本题为二维区间最值查询模板题
 */
static constexpr uint32_t N = 300;
uint32_t A[N][N];
int main() {
    uint32_t m, n;
    while ((cin >> m >> n).m_ok) {
        for (uint32_t i = 0; i < m; i++)
            for (uint32_t j = 0; j < n; j++) cin >> A[i][j];
        // 本题给的空间不大，所以猫树内存池需要复用，每个样例都需要重置内存池
        OY::CatMaxTable2D<uint32_t, N * N * 9 * 9>::s_use_count = 0;
        OY::CatMaxTable2D<uint32_t, N * N * 9 * 9> S(m, n, [](uint32_t i, uint32_t j) {
            return A[i][j];
        });

        // 本题给的空间不大，所以 ST 表内存池需要复用，每个样例都需要重置内存池
        // OY::STMaxTable2D<uint32_t, N * N * 9 * 9>::s_use_count = 0;
        // OY::STMaxTable2D<uint32_t, N * N * 9 * 9> S(m, n, [](uint32_t i, uint32_t j) {
        //     return A[i][j];
        // });

        // Zkw 树的空间倒是很够用
        // auto S = OY::make_ZkwTree2D<uint32_t, 1 << 23>(m, n, std::max<uint32_t>, [](uint32_t i, uint32_t j) {
        //     return A[i][j];
        // });

        uint32_t q;
        cin >> q;
        while (q--) {
            uint32_t r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            auto maxx = S.query(r1 - 1, r2 - 1, c1 - 1, c2 - 1);
            cout << maxx << ' ';
            if (A[r1 - 1][c1 - 1] == maxx || A[r1 - 1][c2 - 1] == maxx || A[r2 - 1][c1 - 1] == maxx || A[r2 - 1][c2 - 1] == maxx)
                cout << "yes\n";
            else
                cout << "no\n";
        }
    }
}