#include "DS/MaskRMQ.h"
#include "DS/SqrtTree.h"
#include "IO/FastIO.h"

/*
[P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)
*/
int main() {
    uint32_t n, m;
    cin >> n >> m;
    OY::CatMaxTable<uint32_t, 1 << 23> S(n, [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    });
    // OY::MaskRMQMaxValueTable<uint32_t, uint64_t, OY::ST::Table, 1 << 18> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // OY::SqrtMaxTable<uint32_t, OY::PrefixTable, OY::ST::Table, 1 << 20> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // OY::STMaxTable<uint32_t, 1 << 23> S(n, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });
    // auto S = OY::make_ZkwTree<uint32_t, 1 << 19>(n, std::max<uint32_t>, [](auto...) {
    //     uint32_t x;
    //     cin >> x;
    //     return x;
    // });

    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << '\n';
    }

    return 0;
}