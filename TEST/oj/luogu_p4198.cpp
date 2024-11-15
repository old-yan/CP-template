#include "DS/OfflineSideView.h"
#include "DS/SideView.h"
#include "IO/FastIO.h"

/*
[P4198 楼房重建](https://www.luogu.com.cn/problem/P4198)
*/
/**
 * 本题意为单点修改点值，查询侧视图能看到的楼房数量
 * 可以套用 SideView 模板
 */

void solve_online() {
    uint32_t n, m;
    cin >> n >> m;
    OY::AscendingSideView<double> S(n);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t x, y;
        cin >> x >> y;
        S.modify(x - 1, double(y) / x);
        cout << S.query(0, n - 1) << endl;
    }
}

void solve_offline() {
    uint32_t n, m;
    cin >> n >> m;
    OY::AscendingOfflineSideView<double> sol(n);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t x, y;
        cin >> x >> y;
        sol.add_modify(x - 1, double(y) / x);
        sol.add_query(n - 1);
    }
    for (auto a : sol.solve()) cout << a << endl;
}

int main() {
    solve_online();
    // solve_offline();
}