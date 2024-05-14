#include "DS/CatTree2D.h"
#include "DS/MaskRMQ.h"
#include "DS/MultiDimSegTree.h"
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
void solve_st2d() {
    uint32_t m, n;
    while ((cin >> m >> n).m_ok) {
        for (uint32_t i = 0; i < m; i++)
            for (uint32_t j = 0; j < n; j++) cin >> A[i][j];
        OY::CatMaxTable2D<uint32_t> S(m, n, [](uint32_t i, uint32_t j) {
            return A[i][j];
        });

        // OY::STMaxTable2D<uint32_t> S(m, n, [](uint32_t i, uint32_t j) {
        //     return A[i][j];
        // });

        // auto S = OY::make_ZkwTree2D<uint32_t>(
        //     m, n, [](uint32_t x, uint32_t y) { return x > y ? x : y; }, [](uint32_t i, uint32_t j) { return A[i][j]; });

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

void solve_mdseg() {
    uint32_t m, n;
    while ((cin >> m >> n).m_ok) {
        for (uint32_t i = 0; i < m; i++)
            for (uint32_t j = 0; j < n; j++) cin >> A[i][j];
        using base_table = OY::MaskRMQMaxValueTable<uint32_t, 9>;
        OY::Segtree2D<uint32_t, uint32_t, base_table, false> S(m * n);
        for (uint32_t i = 0; i < m; i++)
            for (uint32_t j = 0; j < n; j++) S.add_point(A[i][j], i, j);
        S.prepare();

        uint32_t q;
        cin >> q;
        while (q--) {
            uint32_t r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            struct Max {
                uint32_t operator()(uint32_t x, uint32_t y) const { return x > y ? x : y; }
            };
            auto maxx = S.query<Max>(0, r1 - 1, r2 - 1, c1 - 1, c2 - 1);
            cout << maxx << ' ';
            if (A[r1 - 1][c1 - 1] == maxx || A[r1 - 1][c2 - 1] == maxx || A[r2 - 1][c1 - 1] == maxx || A[r2 - 1][c2 - 1] == maxx)
                cout << "yes\n";
            else
                cout << "no\n";
        }
    }
}

int main() {
    solve_st2d();
    // solve_mdseg();
}