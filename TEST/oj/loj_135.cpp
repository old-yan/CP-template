#include "DS/BIT2D.h"
#include "DS/GlobalHashBIT2D.h"
#include "DS/OfflineRectAddRectSumMaintainer2D.h"
#include "IO/FastIO.h"

/*
[#135. 二维树状数组 3：区间修改，区间查询](https://loj.ac/p/135)
*/
/**
 * 本题为二维树状数组模板题
 */

void solve_bit2d() {
    uint32_t n, m;
    cin >> n >> m;
    OY::BIT2D::Tree<int64_t, true> S(n, m);
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int64_t v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            S.add(r1 - 1, r2 - 1, c1 - 1, c2 - 1, v);
        } else {
            uint32_t r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << S.query(r1 - 1, r2 - 1, c1 - 1, c2 - 1) << endl;
        }
    }
}

OY::GHashBIT2D<uint32_t, int64_t, true, false, 4194319> GS;
void solve_hashbit2d() {
    uint32_t n, m;
    cin >> n >> m;
    GS.resize(n, m);
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int64_t v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            GS.add(r1 - 1, r2 - 1, c1 - 1, c2 - 1, v);
        } else {
            uint32_t r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << GS.query(r1 - 1, r2 - 1, c1 - 1, c2 - 1) << endl;
        }
    }
}

using Solver = OY::OFFLINERARSM2D::Solver<uint32_t, int, int64_t>;
OY::GHashBIT2D<uint32_t, Solver::node, false, false, 1 << 23> GS2;
void solve_offline() {
    uint32_t n, m;
    cin >> n >> m;
    Solver sol;
    char op;
    while (cin >> op) {
        if (op == '1') {
            uint32_t r1, c1, r2, c2;
            int64_t v;
            cin >> r1 >> c1 >> r2 >> c2 >> v;
            sol.add_rect(r1, r2, c1, c2, v);
        } else {
            uint32_t r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            sol.add_query(r1, r2, c1, c2);
        }
    }
    for (auto a : sol.solve(GS2)) cout << a << endl;
}

int main() {
    solve_bit2d();
    // solve_hashbit2d();
    // solve_offline();
}