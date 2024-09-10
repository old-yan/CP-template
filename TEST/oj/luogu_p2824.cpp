#include "DS/SortFHQTreap.h"
#include "DS/SortSegTree.h"
#include "IO/FastIO.h"

/*
[P2824 [HEOI2016/TJOI2016] 排序](https://www.luogu.com.cn/problem/P2824)
*/
/**
 * 本题目要求进行在线的区间排序
 * 可以使用 SortSegTree 模板或 SortFHQTreap 模板
 */

void solve_seg() {
    using Tree = OY::SortSegTree<uint32_t>;
    uint32_t n, m;
    cin >> n >> m;
    Tree::_reserve(n * 2 + 500);
    auto key_mapping = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    Tree S(n, key_mapping, n);
    while (m--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '0')
            S.sort<false>(l - 1, r - 1);
        else
            S.sort<true>(l - 1, r - 1);
    }
    uint32_t pos;
    cin >> pos;
    cout << S.get_node(pos - 1)->key() << endl;
}

void solve_fhq() {
    using Tree = OY::SortFHQTreap<uint32_t>;
    uint32_t n, m;
    cin >> n >> m;
    auto key_mapping = [](auto...) {
        uint32_t x;
        cin >> x;
        return x;
    };
    Tree S(n, key_mapping);
    while (m--) {
        char op;
        uint32_t l, r;
        cin >> op >> l >> r;
        if (op == '0')
            S.sort_ascending(l - 1, r - 1);
        else
            S.sort_descending(l - 1, r - 1);
    }
    uint32_t pos;
    cin >> pos;
    cout << S.get_node(pos - 1)->m_key << endl;
}

int main() {
    solve_seg();
    // solve_fhq();
}