#include "DS/LinkBucket.h"
#include "DS/SqrtTree.h"
#include "IO/FastIO.h"
#include "MISC/LazyUmap.h"
#include "TREE/BfnController.h"
#include "TREE/FlatTree.h"

/*
[小Z的树迁移](https://ac.nowcoder.com/acm/problem/281601)
*/
/**
 * 上面的链接打不开
 * 请换用这个链接 https://ac.nowcoder.com/acm/contest/95016/F
 * 两种做法
 * 大家更容易想到的是，树上启发式合并
 *
 * 同时注意到每次查询的搬家目的地都在同一层，所以这也是一个 bfs 序 rmq 问题
 */

void solve_bfn() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, 100000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        S.add_edge(a - 1, b - 1, c);
    }
    S.prepare(), S.set_root(0);

    // 求出根到各点的权值和
    std::vector<uint64_t> to_root(n);
    auto pre_work = [&](uint32_t a, uint32_t p, uint32_t e) {
        if (a) to_root[a] = to_root[p] + e;
    };
    S.tree_dp_edge(0, pre_work, {}, {});

    // 上 bfn 控制器
    OY::BFN::Table<decltype(S)> bfn(&S);
    // bfn 序 rmq
    OY::SqrtMaxTable<uint64_t, OY::SQRT::RandomController<>, 10> T(n, [&](uint32_t i) { return to_root[bfn.m_seq[i]]; });

    uint32_t q;
    cin >> q;
    while (q--) {
        uint32_t cur, d;
        cin >> cur >> d;
        cout << bfn.do_for_descendants(cur - 1, d, [&](auto l, auto r) -> int64_t {
            return l == r + 1 ? -1 : T.query(l, r) - to_root[cur - 1];
        }) << endl;
    }
}

void solve_lazymap() {
    uint32_t n;
    cin >> n;
    OY::FlatTree::Tree<uint32_t, 100000> S(n);
    for (uint32_t i = 1; i != n; i++) {
        uint32_t a, b, c;
        cin >> a >> b >> c;
        S.add_edge(a - 1, b - 1, c);
    }
    S.prepare(), S.set_root(0);

    uint32_t q;
    cin >> q;
    struct Query {
        uint32_t qi, down;
    };
    OY::LBC::LinkBucket<Query> qs(n, q);
    for (uint32_t qi = 0; qi != q; qi++) {
        uint32_t cur, down;
        cin >> cur >> down;
        qs[cur - 1].push_front({qi, down});
    }

    std::vector<int64_t> ans(q, -1);
    // 用 map 存储深度到值的映射
    using T = OY::LazyMappedAddUmap<uint32_t, uint64_t>;
    std::vector<uint32_t> dep(n);
    std::vector<T> trs(n);
    auto pre_work = [&](uint32_t a, uint32_t p, uint32_t e) {
        dep[a] = a ? dep[p] + 1 : 0;
        trs[a].insert_or_assign(dep[a], 0);
    };
    auto report = [&](uint32_t a, uint32_t to, uint32_t e) {
        trs[to].globally_add(e);
        if (trs[a].size() < trs[to].size()) std::swap(trs[a], trs[to]);
        trs[to].enumerate([&](auto k, auto v) {
            trs[a].insert_or_modify(k, [&](auto &old, bool flag) {
                old = flag ? v : std::max(old, v);
            });
        });
    };
    auto after = [&](uint32_t a) {
        for (auto &&[qi, down] : qs[a]) {
            auto it = trs[a].find(dep[a] + down);
            if (it != trs[a].end()) ans[qi] = it->second.m_val;
        }
    };
    S.tree_dp_edge(0, pre_work, report, after);
    for (uint32_t i = 0; i != q; i++) cout << ans[i] << endl;
}

int main() {
    solve_bfn();
    // solve_lazymap();
}