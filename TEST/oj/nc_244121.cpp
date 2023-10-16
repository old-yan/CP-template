#include "IO/FastIO.h"
#include "TREE/AdjDiffTree.h"
#include "TREE/CustomTree.h"

/*
[剖分](https://ac.nowcoder.com/acm/problem/244121)
*/
/**
 * 本题要先进行树上路径增值，子树增值，最后进行单点查询
 * 为树上差分模板题
*/
static constexpr uint32_t N = 10000000;
int main() {
    uint32_t n, m;
    cin >> n >> m;
    // 树的结点数很多，如果把真实的邻接表建出来，空间占用过大
    // 所以没必要建立真实的树，就把每个结点的寻找邻居的逻辑写出来即可
    auto adj_call = [&](uint32_t a, auto call) {
        if (a * 2 + 1 < n) {
            call(a * 2 + 1, 1);
            if (a * 2 + 2 < n) call(a * 2 + 2, 1);
        }
    };
    OY::CustomTree::Tree<uint32_t, decltype(adj_call)> S(n, adj_call);
    S.set_root(0);

    OY::AdjDiffTree::Table<uint32_t, decltype(S), false, N << 1> T(&S, [](auto...) { return 0; });
    T.switch_to_difference_downward();
    OY::AdjDiffTree::Table<uint32_t, decltype(S), false, N << 1> T2(&S, [](auto...) { return 0; });
    T2.switch_to_difference_upward();
    for (uint32_t i = 0; i < m; i++) {
        char op;
        uint32_t x;
        cin >> op >> x;
        if (op == '1')
            T.add_subtree(x - 1, 1);
        else if (op == '2')
            T.add_subtree(0, 1), T.add_subtree(x - 1, -1);
        else if (op == '3')
            T2.add_path(x - 1, 0, -1, 1);
        else
            T.add_subtree(0, 1), T2.add_path(x - 1, 0, -1, -1);
    }
    T.switch_to_value();
    T2.switch_to_value();
    std::vector<uint32_t> cnt(m + 1);
    for (uint32_t i = 0; i < n; i++) cnt[T.query(i) + T2.query(i)]++;
    for (auto a : cnt) cout << a << ' ';
}
