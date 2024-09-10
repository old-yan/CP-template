#include "DS/BIT.h"
#include "DS/Discretizer.h"
#include "DS/LinkBucket.h"
#include "DS/PersistentAVL.h"
#include "DS/PersistentCompressedTree.h"
#include "DS/PersistentSegCounter.h"
#include "DS/PersistentSegTree.h"
#include "DS/StaticBufferWrapWithoutCollect.h"
#include "IO/FastIO.h"

/*
[P3835 【模板】可持久化平衡树](https://www.luogu.com.cn/problem/P3835)
*/
/**
 * 本题为 P3369 可持久化版本
 * 为可持久化平衡树模板题
 * 由于没有在线，所以可以离线处理
 */

static constexpr uint32_t N = 500000;

using PerSegCounter = OY::PerSEGCNT::Table<uint32_t, uint32_t, true, false, false, OY::StaticBufferWrapWithoutCollect<8000000>::type>;
PerSegCounter segcounter_pool[500001];
void solve_persegcounter() {
    static constexpr int M = 1000000001;
    uint32_t n;
    cin >> n;
    int ans = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            segcounter_pool[i] = segcounter_pool[v].copy();
            segcounter_pool[i].add(x + M, 1);
        } else if (opt == '2') {
            segcounter_pool[i] = segcounter_pool[v].copy();
            segcounter_pool[i].remove(x + M);
        } else {
            segcounter_pool[i] = segcounter_pool[v];
            int res = 0;
            if (opt == '3')
                res = segcounter_pool[i].query(0, x + M - 1) + 1;
            else if (opt == '4')
                res = segcounter_pool[i].kth(x - 1)->key() - M;
            else if (opt == '5') {
                uint32_t rank = segcounter_pool[i].query(0, x + M - 1);
                res = rank ? segcounter_pool[i].kth(rank - 1)->key() - M : -2147483647;
            } else {
                uint32_t rank = segcounter_pool[i].query(0, x + M);
                res = rank < segcounter_pool[i].query_all() ? segcounter_pool[i].kth(rank)->key() - M : 2147483647;
            }
            cout << res << endl;
        }
    }
}

using PerCPTree = OY::PerCPTREE::Tree<OY::PerCPTREE::BaseMonoid<uint32_t, 0, std::plus<>>, false, uint32_t, OY::StaticBufferWrapWithoutCollect<8000000>::type>;
PerCPTree cpt_pool[500001];
void solve_percpt() {
    static constexpr int M = 1000000001;
    uint32_t n;
    cin >> n;
    int ans = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            cpt_pool[i] = cpt_pool[v].copy();
            cpt_pool[i].add(x + M, 1);
        } else if (opt == '2') {
            cpt_pool[i] = cpt_pool[v].copy();
            cpt_pool[i].modify(x + M, 0);
        } else {
            auto kth = [&](uint32_t k) {
                return cpt_pool[i].max_right(0, [&](auto v) { return v <= k; }) + 1;
            };
            cpt_pool[i] = cpt_pool[v];
            int res = 0;
            if (opt == '3')
                res = cpt_pool[i].query(0, x + M - 1) + 1;
            else if (opt == '4')
                res = kth(x - 1) - M;
            else if (opt == '5') {
                uint32_t rank = cpt_pool[i].query(0, x + M - 1);
                res = rank ? kth(rank - 1) - M : -2147483647;
            } else {
                uint32_t rank = cpt_pool[i].query(0, x + M);
                res = rank < cpt_pool[i].query_all() ? kth(rank) - M : 2147483647;
            }
            cout << res << endl;
        }
    }
}

struct Node {
    uint32_t ver;
    char op;
    int x;
};
OY::LBC::LinkBucket<Node> buckets;
uint32_t id[N + 1];
int res[N];
void solve_rollback() {
    uint32_t n;
    cin >> n;
    buckets.resize(n + 1, n);
    uint32_t cur = 0;
    // 先读入所有查询，记录好版本之间的依赖关系
    OY::Discretizer<int> D;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t ver;
        char op;
        int x;
        cin >> ver >> op >> x;
        if (op <= '2')
            buckets[id[ver]].push_front(Node{id[i] = ++cur, op, x});
        else
            buckets[id[i] = id[ver]].push_front(Node{i - cur - 1, op, x});
        D << x;
    }
    // 离散化之后，用树状数组模拟平衡树
    D.prepare();
    OY::StaticBIT<uint32_t, false, N * 2> bit(D.size());
    auto dfs = [&](auto self, uint32_t cur, char op, int x) -> void {
        bool flag = false;
        auto rk = D.rank(x);
        if (op == '1')
            bit.add(rk, 1);
        else if (flag = bit.query(rk))
            bit.add(rk, -1);
        for (auto &&[ver, op, x] : buckets[cur]) {
            if (op <= '2')
                self(self, ver, op, x);
            else if (op == '3')
                res[ver] = bit.presum(D.rank(x) - 1) + 1;
            else if (op == '4')
                res[ver] = D[bit.kth(x - 1)];
            else if (op == '5') {
                auto cnt = bit.presum(D.rank(x) - 1);
                res[ver] = cnt ? D[bit.kth(cnt - 1)] : -2147483647;
            } else {
                auto cnt = bit.presum(D.rank(x));
                res[ver] = cnt == bit.query_all() ? 2147483647 : D[bit.kth(cnt)];
            }
        }
        if (op == '1')
            bit.add(rk, -1);
        else if (flag)
            bit.add(rk, 1);
    };
    dfs(dfs, 0, '2', 0);
    for (uint32_t i = 0; i != n - cur; i++) cout << res[i] << endl;
}

using PerAVL = OY::PerAVLMultiset<int, std::less<int>, true, 15000000>;
PerAVL avl_pool[500001];
void solve_peravl() {
    uint32_t n;
    cin >> n;
    int ans = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            avl_pool->unlock();
            avl_pool[i] = avl_pool[v].copy();
            avl_pool[i].insert_by_key(x);
            avl_pool->lock();
        } else if (opt == '2') {
            avl_pool->unlock();
            avl_pool[i] = avl_pool[v].copy();
            avl_pool[i].erase_by_key(x);
            avl_pool->lock();
        } else {
            avl_pool[i] = avl_pool[v];
            int res = 0;
            if (opt == '3')
                res = avl_pool[i].rank(x) + 1;
            else if (opt == '4')
                res = avl_pool[i].kth(x - 1)->get();
            else if (opt == '5') {
                auto p = avl_pool[i].smaller_bound(x);
                res = p->is_null() ? -2147483647 : p->get();
            } else {
                auto p = avl_pool[i].upper_bound(x);
                res = p->is_null() ? 2147483647 : p->get();
            }
            cout << res << endl;
        }
    }
}

using PerSeg = OY::PerSeg::Tree<OY::PerSeg::BaseNode<uint32_t>, OY::PerSeg::Ignore, false, false, uint32_t, OY::StaticBufferWrapWithoutCollect<40000000>::type>;
PerSeg seg_pool[500001];
void solve_perseg() {
    static constexpr int M = 1000000001;
    uint32_t n;
    cin >> n;
    int ans = 0;
    seg_pool[0].resize(M * 2 + 1);
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            seg_pool->unlock();
            seg_pool[i] = seg_pool[v].copy();
            seg_pool[i].add(x + M, 1);
            seg_pool->lock();
        } else if (opt == '2') {
            auto val = seg_pool[v].query(x + M);
            if (val) {
                seg_pool->unlock();
                seg_pool[i] = seg_pool[v].copy();
                seg_pool[i].add(x + M, -1);
                seg_pool->lock();
            } else
                seg_pool[i] = seg_pool[v];
        } else {
            seg_pool[i] = seg_pool[v];
            int res = 0;
            if (opt == '3') {
                res = seg_pool[i].query(0, x + M - 1) + 1;
            } else if (opt == '4') {
                res = seg_pool[i].kth(x - 1).m_index - M;
            } else if (opt == '5') {
                uint32_t rank = seg_pool[i].query(0, x + M - 1);
                res = rank ? seg_pool[i].kth(rank - 1).m_index - M : -2147483647;
            } else {
                uint32_t rank = seg_pool[i].query(0, x + M);
                res = rank < seg_pool[i].query_all() ? seg_pool[i].kth(rank).m_index - M : 2147483647;
            }
            cout << res << endl;
        }
    }
}

int main() {
    solve_persegcounter();
    // solve_percpt();
    // solve_rollback();
    // solve_peravl();
    // solve_perseg();
}
