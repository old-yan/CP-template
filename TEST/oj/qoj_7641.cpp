#include "DS/GlobalHashMap.h"
#include "DS/RangeManager.h"
#include "DS/SegTree.h"
#include <iostream>

/*
[J. Range Sets](https://qoj.ac/contest/1399/problem/7641)
*/
/**
 * 本题可以作为 RangeManager 的模板题
 * 展现出回调函数的威力
 */

OY::GHASH::UnorderedMap<uint32_t, OY::RangeManager<uint32_t> *, false, 19> rms;
void solve_rm() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    uint32_t n, q;
    std::cin >> n >> q;
    // 维护差分树
    // 差分数组的原数组的第 i 个数表示集合 i 包含的点数
    OY::Seg::Tree<OY::Seg::BaseNode<uint32_t>, OY::Seg::Ignore, false, uint32_t, 1 << 23> S(n + 2);
    auto add_call = [&](uint32_t l, uint32_t r) { S.add(l, 1), S.add(r + 1, -1); };
    auto remove_call = [&](uint32_t l, uint32_t r) { S.add(l, -1), S.add(r + 1, 1); };
    for (uint32_t i = 0; i < q; i++) {
        char op;
        std::cin >> op;
        if (op == '+') {
            uint32_t l, r, x;
            std::cin >> l >> r >> x;
            auto p = rms.insert(x).m_ptr;
            if (!p->m_mapped) p->m_mapped = new OY::RangeManager<uint32_t>;
            p->m_mapped->add_range({l, r}, add_call, remove_call);
        } else if (op == '-') {
            uint32_t l, r, x;
            std::cin >> l >> r >> x;
            auto p = rms.find(x);
            if (p) p->m_mapped->remove_range({l, r}, add_call, remove_call);
        } else {
            uint32_t k;
            std::cin >> k;
            std::cout << S.query(0, k) << std::endl;
        }
    }
}

int main() {
    solve_rm();
}
