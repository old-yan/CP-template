#include "DS/SideView.h"
#include "IO/FastIO.h"

/*
[P4198 楼房重建](https://www.luogu.com.cn/problem/P4198)
*/
/**
 * 本题意为单点修改点值，查询侧视图能看到的楼房数量
 * 可以套用 SideView 模板
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    struct fraction {
        uint32_t a, b;
        bool operator<(const fraction &rhs) const {
            return uint64_t(a) * rhs.b < uint64_t(b) * rhs.a;
        }
    };
    OY::SV::Solver<fraction> sol(n);
    for (uint32_t i = 0; i != m; i++) {
        uint32_t x, y;
        cin >> x >> y;
        // 为避免浮点数精度问题，用 y x 两个数字来保存一个分数 y/x
        sol.add_modify(x - 1, {y, x});
        sol.add_query(n - 1);
    }
    // 最小值为 0/1
    // 最大值为 1/0
    for (auto a : sol.solve({0, 1}, {1, 0})) cout << a << endl;
}