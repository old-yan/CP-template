#include "DS/FHQTreap.h"
#include "DS/GlobalHashBIT.h"
#include "DS/LinkBucket.h"
#include "DS/PersistentFHQTreap.h"
#include "DS/PersistentSegTree.h"
#include "DS/SegTree.h"
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
struct Node {
    uint32_t ver;
    char op;
    int x;
};
OY::LBC::LinkBucket<Node, N + 1, N> buckets;
uint32_t id[N + 1];
int res[N];
void solve_rollbackfhq() {
    uint32_t n;
    cin >> n;
    buckets.resize(n + 1, n);
    uint32_t cur = 0;
    // 先读入所有查询，记录好版本之间的依赖关系
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t ver;
        char op;
        int x;
        cin >> ver >> op >> x;
        if (op <= '2') {
            buckets[id[ver]].push_back(Node{id[i] = ++cur, op, x});
        } else {
            buckets[id[ver]].push_back(Node{i - cur - 1, op, x});
            id[i] = id[ver];
        }
    }
    OY::FHQTreap<int, std::less<int>, N + 1> S;
    auto dfs = [&](auto self, uint32_t cur, char op, int x) -> void {
        bool flag = false;
        if (op == '1')
            S.insert_by_key(x);
        else
            flag = S.erase_by_key(x);
        for (auto &&[ver, op, x] : buckets[cur]) {
            if (op <= '2')
                self(self, ver, op, x);
            else if (op == '3')
                res[ver] = S.rank(x) + 1;
            else if (op == '4')
                res[ver] = S.kth(x - 1)->get();
            else if (op == '5') {
                auto p = S.smaller_bound(x);
                res[ver] = p->is_null() ? -2147483647 : p->get();
            } else {
                auto p = S.upper_bound(x);
                res[ver] = p->is_null() ? 2147483647 : p->get();
            }
        }
        if (op == '1')
            S.erase_by_key(x);
        else if (flag)
            S.insert_by_key(x);
    };
    dfs(dfs, 0, '2', 0);
    for (uint32_t i = 0; i != n - cur; i++) cout << res[i] << endl;
}

void solve_rollbackseg() {
    static constexpr int M = 1000000001;
    uint32_t n;
    cin >> n;
    buckets.resize(n + 1, n);
    uint32_t cur = 0;
    // 先读入所有查询，记录好版本之间的依赖关系
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t ver;
        char op;
        int x;
        cin >> ver >> op >> x;
        if (op <= '2') {
            buckets[id[ver]].push_back(Node{id[i] = ++cur, op, x});
        } else {
            buckets[id[ver]].push_back(Node{i - cur - 1, op, x});
            id[i] = id[ver];
        }
    }
    OY::Seg::Tree<OY::Seg::BaseNode<uint32_t>, OY::Seg::Ignore, false, uint32_t, N << 5> S(M * 2 + 1);
    auto dfs = [&](auto self, uint32_t cur, char op, int x) -> void {
        bool flag = false;
        if (op == '1')
            S.add(x + M, 1);
        else if (flag = S.query(x + M))
            S.add(x + M, -1);
        for (auto &&[ver, op, x] : buckets[cur]) {
            if (op <= '2')
                self(self, ver, op, x);
            else if (op == '3')
                res[ver] = S.query(0, x + M - 1) + 1;
            else if (op == '4')
                res[ver] = S.kth(x - 1) - M;
            else if (op == '5') {
                uint32_t rank = S.query(0, x + M - 1);
                res[ver] = rank ? S.kth(rank - 1) - M : -2147483647;
            } else {
                uint32_t rank = S.query(0, x + M);
                res[ver] = rank < S.query_all() ? S.kth(rank) - M : 2147483647;
            }
        }
        if (op == '1')
            S.add(x + M, -1);
        else if (flag)
            S.add(x + M, 1);
    };
    dfs(dfs, 0, '2', 0);
    for (uint32_t i = 0; i != n - cur; i++) cout << res[i] << endl;
}

static constexpr int GM = 1000000001;
OY::GHashBIT<uint32_t, uint32_t, false, false, 22> GS(GM * 2 + 1);
void solve_rollbackhashbit() {
    uint32_t n;
    cin >> n;
    buckets.resize(n + 1, n);
    uint32_t cur = 0;
    // 先读入所有查询，记录好版本之间的依赖关系
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t ver;
        char op;
        int x;
        cin >> ver >> op >> x;
        if (op <= '2') {
            buckets[id[ver]].push_back(Node{id[i] = ++cur, op, x});
        } else {
            buckets[id[ver]].push_back(Node{i - cur - 1, op, x});
            id[i] = id[ver];
        }
    }
    auto dfs = [&](auto self, uint32_t cur, char op, int x) -> void {
        bool flag = false;
        if (op == '1')
            GS.add(x + GM, 1);
        else if (flag = GS.query(x + GM))
            GS.add(x + GM, -1);
        for (auto &&[ver, op, x] : buckets[cur]) {
            if (op <= '2')
                self(self, ver, op, x);
            else if (op == '3')
                res[ver] = GS.presum(x + GM - 1) + 1;
            else if (op == '4')
                res[ver] = GS.kth(x - 1) - GM;
            else if (op == '5') {
                uint32_t rank = GS.presum(x + GM - 1);
                res[ver] = rank ? GS.kth(rank - 1) - GM : -2147483647;
            } else {
                uint32_t rank = GS.presum(x + GM);
                res[ver] = rank < GS.query_all() ? GS.kth(rank) - GM : 2147483647;
            }
        }
        if (op == '1')
            GS.add(x + GM, -1);
        else if (flag)
            GS.add(x + GM, 1);
    };
    dfs(dfs, 0, '2', 0);
    for (uint32_t i = 0; i != n - cur; i++) cout << res[i] << endl;
}

using PerFHQ = OY::PerFHQTreap<int, std::less<int>, true, 20000000>;
PerFHQ fhq_pool[500001];
void solve_perfhq() {
    uint32_t n;
    cin >> n;
    int ans = 0;
    for (uint32_t i = 1; i <= n; i++) {
        uint32_t v;
        char opt;
        int x;
        cin >> v >> opt >> x;
        if (opt == '1') {
            fhq_pool->unlock();
            fhq_pool[i] = fhq_pool[v].copy();
            fhq_pool[i].insert_by_key(x);
            fhq_pool->lock();
        } else if (opt == '2') {
            fhq_pool->unlock();
            fhq_pool[i] = fhq_pool[v].copy();
            fhq_pool[i].erase_by_key(x);
            fhq_pool->lock();
        } else {
            fhq_pool[i] = fhq_pool[v];
            int res = 0;
            if (opt == '3')
                res = fhq_pool[i].rank(x) + 1;
            else if (opt == '4')
                res = fhq_pool[i].kth(x - 1)->get();
            else if (opt == '5') {
                auto p = fhq_pool[i].smaller_bound(x);
                res = p->is_null() ? -2147483647 : p->get();
            } else {
                auto p = fhq_pool[i].upper_bound(x);
                res = p->is_null() ? 2147483647 : p->get();
            }
            cout << res << endl;
        }
    }
}

using PerSeg = OY::PerSeg::Tree<OY::PerSeg::BaseNode<uint32_t>, OY::PerSeg::Ignore, false, false, uint32_t, 40000000>;
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
                res = seg_pool[i].kth(x - 1) - M;
            } else if (opt == '5') {
                uint32_t rank = seg_pool[i].query(0, x + M - 1);
                res = rank ? seg_pool[i].kth(rank - 1) - M : -2147483647;
            } else {
                uint32_t rank = seg_pool[i].query(0, x + M);
                res = rank < seg_pool[i].query_all() ? seg_pool[i].kth(rank) - M : 2147483647;
            }
            cout << res << endl;
        }
    }
}

int main() {
    solve_rollbackfhq();
    // solve_rollbackhashbit();
    // solve_rollbackseg();
    // solve_perfhq();
    // solve_perseg();
}
