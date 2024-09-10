#include "DS/MonoAVL.h"
#include "IO/FastIO.h"

/*
[P3391 【模板】文艺平衡树](https://www.luogu.com.cn/problem/P3391)
*/
/**
 * 本题需要一种支持分裂、合并、区间修改的数据结构
 * 线段树支持区间修改，但是线段树的形态是定死的，不能进行左右孩子交换的修改
 * 只能选择二叉平衡树
 */

int main() {
    uint32_t n, m;
    cin >> n >> m;
    auto S = OY::MonoAVLSequence<uint32_t, true, 100000>::from_mapping(n, [&](uint32_t i) { return i + 1; });
    using node = decltype(S)::node;
    for (auto i = 0; i < m; i++) {
        uint32_t l, r;
        cin >> l >> r;
        S.reverse(l - 1, r - 1);
    }
    S.enumerate([](node *p) {
        cout << p->m_val << ' ';
    });
}