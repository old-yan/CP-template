#include "DS/LinkBucket.h"
#include "DS/PersistentAVL.h"
#include "DS/PersistentSegTree.h"
#include "IO/FastIO.h"

#include <limits.h>

/*
[P3919 【模板】可持久化线段树 1（可持久化数组）](https://www.luogu.com.cn/problem/P3919)
*/
/**
 * 本题为可持久化线段树模板题
 * 由于不强制在线，所以可以离线回滚操作
 */

static constexpr uint32_t N = 1000000, M = 1000000;
struct Node {
    uint32_t ver, is_query, loc;
    int val;
};
OY::LBC::LinkBucket<Node> buckets;
uint32_t id[M + 1];
int arr[N + 1];
void solve_rollback() {
    uint32_t n, m;
    cin >> n >> m;
    buckets.resize(m + 1, m);
    std::vector<int> res(m + 1, INT_MAX);
    for (uint32_t i = 1; i <= n; i++) cin >> arr[i];
    for (uint32_t i = 1; i <= m; i++) {
        uint32_t v, loc;
        char op;
        cin >> v >> op >> loc;
        if (op == '1') {
            int val;
            cin >> val;
            buckets[id[v]].push_front(Node{i, false, loc, val});
            id[i] = i;
        } else {
            buckets[id[v]].push_front(Node{i, true, loc});
            id[i] = id[v];
        }
    }
    auto dfs = [&](auto self, uint32_t cur, bool is_q, uint32_t loc, int val) -> void {
        if (is_q)
            res[cur] = arr[loc];
        else
            std::swap(arr[loc], val);
        for (auto &&[ver, is_q, loc, val] : buckets[cur]) self(self, ver, is_q, loc, val);
        if (!is_q) arr[loc] = val;
    };
    dfs(dfs, 0, false, 0, 0);
    for (auto x : res)
        if (x != INT_MAX) cout << x << endl;
}

using PerSeg = OY::StaticPerSegSumTree<int, false, true, uint32_t, 25000000>;
PerSeg seg_pool[1000001];
void solve_perseg() {
    uint32_t n, m;
    cin >> n >> m;
    seg_pool[0].resize(n, [](auto...) {
        int x;
        cin >> x;
        return x;
    });
    for (uint32_t i = 1; i <= m; i++) {
        uint32_t v, loc;
        char op;
        cin >> v >> op >> loc;
        if (op == '1') {
            int val;
            cin >> val;
            seg_pool->unlock();
            (seg_pool[i] = seg_pool[v].copy()).modify(loc - 1, val);
            seg_pool->lock();
        } else
            cout << (seg_pool[i] = seg_pool[v]).query(loc - 1) << '\n';
    }
}

template <typename Node>
struct NodeWrap {
    using key_type = int;
    int m_key;
    void set(const key_type &key) { m_key = key; }
    const int &get() const { return m_key; }
};
using AVL = OY::PerAVL::Tree<NodeWrap, true, 1 << 26>;
using node = AVL::node;
AVL avl_pool[1000001];
void solve_peravl() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) {
        int x;
        cin >> x;
        avl_pool[0].insert_by_rank(x, avl_pool[0].size());
    }
    for (uint32_t i = 1; i <= m; i++) {
        uint32_t v, loc;
        char op;
        cin >> v >> op >> loc;
        if (op == '1') {
            int val;
            cin >> val;
            avl_pool->unlock();
            (avl_pool[i] = avl_pool[v].copy()).modify_by_rank(loc - 1, [&](node *p) { p->set(val); });
            avl_pool->lock();
        } else {
            cout << (avl_pool[i] = avl_pool[v]).kth(loc - 1)->get() << '\n';
        }
    }
}

int main() {
    solve_rollback();
    // solve_perseg();
    // solve_peravl();
}
