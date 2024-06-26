#include "DS/CatTree.h"
#include "DS/MaskRMQ.h"
#include "DS/RollbackCatTree.h"
#include "DS/RollbackSTTable.h"
#include "DS/RollbackSqrtTree.h"
#include "DS/STTable.h"
#include "DS/SegTree.h"
#include "DS/SqrtTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)
*/
/**
 * 本题要进行区间最值查询，为 RMQ 模板题
 * 可以使用 MaskRMQ SqrtTree STTable CatTree ZkwTree SegTree 等数据结构
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto my_max = [](auto x, auto y) { return std::max(x, y); };
    OY::SqrtMaxTable<uint32_t, OY::Sqrt::RandomController<>, 12> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    // auto S = [n]() {
    //     OY::RollbackSqrtMaxTable<uint32_t, OY::RollbackSqrt::RandomController<>, 12> S;
    //     // OY::RollbackCatMaxTable<uint32_t, 18> S;
    //     // OY::RollbackSTMaxTable<uint32_t, 18> S;
    //     S.reserve(n);
    //     for (uint32_t i = 0; i != n; i++) {
    //         uint32_t x;
    //         cin >> x;
    //         S.push_back(x);
    //     }
    //     return S;
    // }();
    // OY::MaskRMQMaxValueTable<uint32_t, 11> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // OY::STMaxTable<uint32_t, 17> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // OY::CatMaxTable<uint32_t, 17> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = OY::make_ZkwTree<uint32_t>(n, std::max<uint32_t>, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = OY::make_SegTree<uint32_t, true, OY::Seg::Ignore, uint32_t>(n, my_max, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });

    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << '\n';
    }
}
