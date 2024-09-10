#include "DS/CatTree.h"
#include "DS/MaskRMQ.h"
#include "DS/RollbackCatTree.h"
#include "DS/RollbackSparseTable.h"
#include "DS/RollbackSqrtTree.h"
#include "DS/SparseTable.h"
#include "DS/SqrtTree.h"
#include "DS/MonoZkwTree.h"
#include "IO/FastIO.h"

/*
[P3865 【模板】ST 表](https://www.luogu.com.cn/problem/P3865)
*/
/**
 * 本题要进行区间最值查询，为 RMQ 模板题
 * 可以使用 MaskRMQ SqrtTree STTable CatTree ZkwTree SegTree 等数据结构
 */

static constexpr uint32_t N = 100000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto my_max = [](auto x, auto y) { return std::max(x, y); };
    auto read = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    OY::SqrtMaxTable<uint32_t, OY::SQRT::RandomController<>, 12> S(n, read);
    // OY::MaskRMQMaxValueTable<uint32_t, 11> S(n, read);
    // OY::STMaxTable<uint32_t, 17> S(n, read);
    // OY::CatMaxTable<uint32_t, 17> S(n, read);
    // OY::MonoMaxTree<uint32_t> S(n, read);

    for (uint32_t i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        cout << S.query(l - 1, r - 1) << '\n';
    }
}
