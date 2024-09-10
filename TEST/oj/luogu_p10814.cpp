#include "DS/MergeSortTree.h"
#include "DS/OfflinePointAddRectSumCounter2D.h"
#include "DS/PointAddRectSumCounter2D.h"
#include "DS/WTree.h"
#include "DS/WaveLet.h"
#include "IO/FastIO.h"

/*
[P10814 【模板】离线二维数点](https://www.luogu.com.cn/problem/P10814)
*/
/**
 * 本题名义上说可以离线，但是在线也不难，可以使用小波树或者 MergeSortTree
 */

static constexpr uint32_t N = 2000000, Q = 2000000;
uint32_t arr[N], ans[Q];
struct Query {
    uint32_t pos, index, val;
} qs[Q * 2 + 1];
void solve_offline() {
    uint32_t n, q;
    cin >> n >> q;
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];
    uint32_t qcnt = 0;
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r, x;
        cin >> l >> r >> x;
        if (l > 1) qs[qcnt++] = {l - 2, i, x};
        qs[qcnt++] = {r - 1, q + i, x};
    }
    std::sort(qs, qs + qcnt, [](auto &x, auto &y) { return x.pos < y.pos; });
    qs[qcnt].pos = n;
    OY::WTree::Tree<uint32_t> W(2000001);
    for (uint32_t i = 0, j = 0; i != n; i++) {
        W.add(arr[i], 1);
        while (qs[j].pos == i) {
            auto &&[_, qi, val] = qs[j++];
            if (qi < q)
                ans[qi] -= W.presum(val);
            else
                ans[qi - q] += W.presum(val);
        }
    }
    for (uint32_t i = 0; i != q; i++) cout << ans[i] << endl;
}

void solve_ds() {
    uint32_t n, q;
    cin >> n >> q;
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::WaveLet::Table<uint32_t> S(n, read);
    // OY::WaveLet::Tree<uint32_t> S(n, read);
    // OY::MS::Tree<uint32_t> S(n, read);
    while (q--) {
        uint32_t l, r, x;
        cin >> l >> r >> x;
        cout << S.rank(l - 1, r - 1, x + 1) << endl;
    }
}

void solve_offlineparsc() {
    uint32_t n, q;
    cin >> n >> q;
    OY::OFFLINEPARSC2D::Solver<uint32_t> sol(n, q);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        sol.add_point(i, x);
    }
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r, x;
        cin >> l >> r >> x;
        sol.add_query(l - 1, r - 1, 0, x);
    }
    for (auto a : sol.solve<uint32_t, OY::WTree::Tree<uint32_t>>()) cout << a << endl;
}

void solve_parsc() {
    uint32_t n, q;
    cin >> n >> q;
    OY::PARSC2D::Table<uint32_t> S(n);
    for (uint32_t i = 0; i != n; i++) {
        uint32_t x;
        cin >> x;
        S.add_point(i, x);
    }
    S.prepare();
    for (uint32_t i = 0; i != q; i++) {
        uint32_t l, r, x;
        cin >> l >> r >> x;
        cout << S.query(l - 1, r - 1, 0, x) << endl;
    }
}

int main() {
    solve_offline();
    // solve_ds();
    // solve_offlineparsc();
    // solve_parsc();
}