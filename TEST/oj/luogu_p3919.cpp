#include "DS/PersistentFHQTreap.h"
#include "DS/PersistentSegTree.h"
#include "IO/FastIO.h"

/*
[P3919 【模板】可持久化线段树 1（可持久化数组）](https://www.luogu.com.cn/problem/P3919)
*/
/**
 * 本题为可持久化线段树模板题
 */
using PerSeg = OY::PerSeg::Tree<OY::PerSeg::BaseNode<int>, OY::PerSeg::Ignore, false, true, uint32_t, 25000000>;
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
using FHQ = OY::PerFHQ::Multiset<NodeWrap, true, 1 << 26>;
using node = FHQ::node;
FHQ fhq_pool[1000001];
void solve_perfhq() {
    uint32_t n, m;
    cin >> n >> m;
    for (uint32_t i = 0; i < n; i++) {
        int x;
        cin >> x;
        fhq_pool[0].insert_by_rank(x, fhq_pool[0].size());
    }
    for (uint32_t i = 1; i <= m; i++) {
        uint32_t v, loc;
        char op;
        cin >> v >> op >> loc;
        if (op == '1') {
            int val;
            cin >> val;
            fhq_pool->unlock();
            (fhq_pool[i] = fhq_pool[v].copy()).modify_by_rank(loc - 1, [&](node *p) { p->set(val); });
            fhq_pool->lock();
        } else {
            cout << (fhq_pool[i] = fhq_pool[v]).kth(loc - 1)->get() << '\n';
        }
    }
}

int main() {
    solve_perseg();
    // solve_perfhq();
}