#include "DS/CatTree2D.h"
#include "DS/MaskRMQ.h"
#include "DS/MonoZkwTree2D.h"
#include "DS/MultiDimSegTree.h"
#include "DS/RMQ2D.h"
#include "DS/SparseTable2D.h"
#include "IO/FastIO.h"

/*
[Check Corners](http://acm.hdu.edu.cn/showproblem.php?pid=2888)
*/
/**
 * 本题为二维区间最值查询模板题
 */

static constexpr uint32_t N = 300;
uint32_t A[N][N];
template <typename Table>
void solve_st2d() {
    uint32_t m, n;
    while ((cin >> m >> n).m_ok) {
        for (uint32_t i = 0; i < m; i++)
            for (uint32_t j = 0; j < n; j++) cin >> A[i][j];
        Table S(m, n, [](uint32_t i, uint32_t j) {
            return A[i][j];
        });
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
    while (cin >> m >> n) {
        for (uint32_t i = 0; i < m; i++)
            for (uint32_t j = 0; j < n; j++) cin >> A[i][j];
        using base_table = OY::MaskRMQMaxValueTable<uint32_t, 11>;
        struct monoid {
            using value_type = uint32_t;
            static value_type op(value_type x, value_type y) { return std::max(x, y); }
            static value_type identity() { return 0; }
        };
        OY::MonoMaxMDSeg<uint32_t, base_table, 2, false, monoid> S(m * n);
        for (uint32_t i = 0; i < m; i++)
            for (uint32_t j = 0; j < n; j++) S.add_point(A[i][j], i, j);
        S.prepare();

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

int main() {
    solve_st2d<OY::CatMaxTable2D<uint32_t>>();
    // solve_st2d<OY::STMaxTable2D<uint32_t>>();
    // solve_st2d<OY::RMQMaxTable2D<uint32_t>>();
    // solve_st2d<OY::MonoMaxTree2D<uint32_t>>();
    // solve_mdseg();
}