#include "DS/ReversedSegCounter.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"
#include "TREE/FlatTree.h"

/*
[P6018 [Ynoi2010] Fusion tree](https://www.luogu.com.cn/problem/P6018)
*/
/**
 * 本题可以使用 ReverseSegCounter 模拟解决
 */

void solve_revsegcounter() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<bool, 500000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];

    // 把邻居分成父和子两部分处理
    using Trie = OY::REVSEGCNT::Table<uint32_t, uint32_t, false, true, true, OY::StaticBufferWrapWithCollect<1000000>::type>;
    // son_tries[i] 保存 i 的每个孩子
    std::vector<Trie> son_tries(n + 1);
    // lazy[i] 表示 i 对 i 的每个孩子的懒增量
    std::vector<uint32_t> fa(n), lazy(n + 1);
    auto pre = [&](uint32_t a, uint32_t p) {
        p = ~p ? p : n;
        fa[a] = p;
        son_tries[p].add(arr[a], 1);
    };
    S.tree_dp_vertex(0, pre, {}, {});

    auto modify = [&](uint32_t i, uint32_t inc) {
        uint32_t p = fa[i];
        uint32_t now = arr[i] + lazy[p];
        son_tries[p].add(now, -1);
        son_tries[p].add(now + inc, 1);
        arr[i] += inc;
    };
    while (m--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            x--;
            if (x) modify(fa[x], 1);
            son_tries[x].globally_plus_one();
            lazy[x]++;
        } else if (op == '2') {
            uint32_t x, v;
            cin >> x >> v;
            x--;
            modify(x, -v);
        } else {
            uint32_t x;
            cin >> x;
            x--;
            uint64_t ans = 0;
            if (x) ans = arr[fa[x]] + lazy[fa[fa[x]]];
            ans ^= son_tries[x].key_xorsum();
            cout << ans << endl;
        }
    }
}

void solve_revsegcounter2() {
    uint32_t n, m;
    cin >> n >> m;
    OY::FlatTree::Tree<bool, 500000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b;
        cin >> a >> b;
        S.add_edge(a - 1, b - 1);
    }
    S.prepare(), S.set_root(0);
    std::vector<uint32_t> arr(n);
    for (uint32_t i = 0; i != n; i++) cin >> arr[i];

    // 把邻居分成父和子两部分处理
    using Trie = OY::REVSEGCNT::Table<uint32_t, uint32_t, false, true, true, OY::StaticBufferWrapWithCollect<1000000>::type>;
    using node = Trie::node;
    // son_tries[i] 保存 i 的每个孩子
    std::vector<Trie> son_tries(n + 1);
    std::vector<uint32_t> fa(n);
    // iters[i] 表示 i 在 son_tries[i] 里的结点
    std::vector<node *> iters(n);
    auto pre = [&](uint32_t a, uint32_t p) {
        fa[a] = p;
        p = ~p ? p : n;
        son_tries[p].add(arr[a], 1);
        iters[a] = son_tries[p].find(arr[a]);
    };
    S.tree_dp_vertex(0, pre, {}, {});

    auto modify = [&](uint32_t i, uint32_t inc) {
        auto it = iters[i];
        it->fetch();
        uint32_t old = it->key();
        son_tries[i ? fa[i] : n].add(old, -1);
        son_tries[i ? fa[i] : n].add(old + inc, 1);
        iters[i] = son_tries[i ? fa[i] : n].find(old + inc);
    };
    while (m--) {
        char op;
        cin >> op;
        if (op == '1') {
            uint32_t x;
            cin >> x;
            x--;
            if (uint32_t p = fa[x]; ~p) modify(p, 1);
            son_tries[x].globally_plus_one();
        } else if (op == '2') {
            uint32_t x, v;
            cin >> x >> v;
            x--;
            modify(x, -v);
        } else {
            uint32_t x;
            cin >> x;
            x--;
            uint64_t ans = 0;
            if (uint32_t p = fa[x]; ~p) {
                auto it = iters[p];
                it->fetch();
                ans = it->key();
            }
            ans ^= son_tries[x].key_xorsum();
            cout << ans << endl;
        }
    }
}

int main() {
    solve_revsegcounter();
    // solve_revsegcounter2();
}