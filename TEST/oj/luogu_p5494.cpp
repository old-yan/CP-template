#include "DS/AVL.h"
#include "DS/FHQCounter.h"
#include "DS/SegCounter.h"
#include "DS/SegTree.h"
#include "DS/Splay.h"
#include "DS/StaticBufferWrapWithCollect.h"
#include "IO/FastIO.h"

/*
[P5494 【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)
*/
/**
 * 本题为线段树分裂/合并模板题
 * 涉及到分裂合并，只能使用动态开点版本，不能使用 Zkw
 */

static constexpr uint32_t N = 200000, M = 200000;
void solve_counter() {
    uint32_t n, m, cnt = 2;
    cin >> n >> m;
    using Counter = OY::SEGCNT::Table<uint32_t, uint64_t, true, false, false, OY::StaticBufferWrapWithCollect<(N + M) << 1>::type>;
    // using Counter = OY::FHQCNT::Table<uint32_t, uint64_t, true, false, OY::StaticBufferWrapWithCollect<N + M>::type>;
    std::vector<Counter> counter_pool(m + 2);
    for (uint32_t i = 0; i != n; i++) {
        uint64_t x;
        cin >> x;
        counter_pool[1].add(i, x);
    }
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') { // 加新
            uint32_t p, x, y;
            cin >> p >> x >> y;
            auto S3 = counter_pool[p].split_by_key(y);
            counter_pool[cnt++] = counter_pool[p].split_by_key(x - 1);
            counter_pool[p].merge(S3);
        } else if (op == '1') { // 合并
            uint32_t p, t;
            cin >> p >> t;
            counter_pool[p].merge(counter_pool[t]);
        } else if (op == '2') { // 添加
            uint32_t p, q;
            uint64_t x;
            cin >> p >> x >> q;
            counter_pool[p].add(q - 1, x);
        } else if (op == '3') { // 查询
            uint32_t p, x, y;
            cin >> p >> x >> y;
            cout << counter_pool[p].query(x - 1, y - 1) << endl;
        } else { // 查询
            uint32_t p;
            uint64_t k;
            cin >> p >> k;
            if (counter_pool[p].query_all() < k)
                cout << "-1\n";
            else
                cout << counter_pool[p].kth(k - 1)->key() + 1 << '\n';
        }
    }
}

using Seg = OY::Seg::Tree<OY::Seg::BaseNode<uint64_t>, OY::Seg::Ignore, uint32_t, OY::StaticBufferWrapWithCollect<1 << 22>::type>;
Seg seg_pool[M + 2];
void solve_seg() {
    uint32_t n, m, cnt = 2;
    cin >> n >> m;
    auto read = [](auto...) {
        uint64_t x;
        cin >> x;
        return x;
    };
    seg_pool[1].resize(n, read);
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') { // 加新
            uint32_t p, x, y;
            cin >> p >> x >> y;
            auto S3 = seg_pool[p].split_by_key(y);
            seg_pool[cnt++] = seg_pool[p].split_by_key(x - 1);
            seg_pool[p].merge(S3);
        } else if (op == '1') { // 合并
            uint32_t p, t;
            cin >> p >> t;
            seg_pool[p].merge(seg_pool[t]);
        } else if (op == '2') { // 添加
            uint32_t p, q;
            uint64_t x;
            cin >> p >> x >> q;
            seg_pool[p].add(q - 1, x);
        } else if (op == '3') { // 查询
            uint32_t p, x, y;
            cin >> p >> x >> y;
            cout << seg_pool[p].query(x - 1, y - 1) << endl;
        } else { // 查询
            uint32_t p;
            uint64_t k;
            cin >> p >> k;
            if (seg_pool[p].query_all() < k)
                cout << "-1\n";
            else
                cout << seg_pool[p].kth(k - 1).m_index + 1 << '\n';
        }
    }
}

template <typename Node>
struct NodeWrap {
    using key_type = uint32_t;
    uint32_t m_val;
    uint32_t m_count;
    uint64_t m_sum;
    void pushup(Node *lchild, Node *rchild) {
        m_sum = m_count;
        if (lchild) m_sum += lchild->m_sum;
        if (rchild) m_sum += rchild->m_sum;
    }
    uint32_t get() const { return m_val; }
    void set(uint32_t v) { m_val = v; }
};
void solve_splay() {
    using Splay = OY::SPLAY::Tree<NodeWrap>;
    using node = Splay::node;
    Splay splay_pool[200002];
    Splay::_reserve(750000);
    uint32_t n, m, id = 1;
    cin >> n >> m;
    auto add = [&](uint32_t i, uint32_t key, uint64_t val) {
        if (val && !splay_pool[i].modify_by_key(key, [val](node *p) { p->m_count += val; })) {
            splay_pool[i].insert_by_key(key, [val](node *p) { p->m_count = val; });
        }
    };
    for (uint32_t i = 1; i <= n; i++)
        splay_pool[1].insert_by_key(i, [](node *p) { cin >> p->m_count; });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, l, r;
            cin >> a >> l >> r;
            auto S = splay_pool[a].split_by_key(r + 1);
            splay_pool[++id] = splay_pool[a].split_by_key(l);
            splay_pool[a].join(S);
        } else if (op == '1') {
            uint32_t a, b;
            cin >> a >> b;
            splay_pool[a].merge(splay_pool[b], [](auto p, auto q) {
                p->m_count += q->m_count;
            });
        } else if (op == '2') {
            uint32_t a, v;
            uint64_t cnt;
            cin >> a >> cnt >> v;
            add(a, v, cnt);
        } else if (op == '3') {
            uint32_t a, l, r;
            cin >> a >> l >> r;
            auto S2 = splay_pool[a].split_by_key(r + 1);
            auto S1 = splay_pool[a].split_by_key(l);
            cout << S1.root()->m_sum << '\n';
            splay_pool[a].join(S1), splay_pool[a].join(S2);
        } else {
            uint32_t a;
            uint64_t k;
            cin >> a >> k;
            if (splay_pool[a].root()->m_sum < k)
                cout << "-1\n";
            else {
                struct Getter {
                    using value_type = uint64_t;
                    value_type operator()() const { return 0; }
                    void tree(value_type &x, node *y) const { x += y->m_sum; }
                    void node(value_type &x, node *y) const { x += y->m_count; }
                };
                auto pos = splay_pool[a].max_right<Getter>(0, [&](auto v) { return v < k; }) + 1;
                cout << splay_pool[a].kth(pos)->m_val << '\n';
            }
        }
    }
}

void solve_avl() {
    using AVL = OY::AVL::Tree<NodeWrap>;
    using node = AVL::node;
    AVL avl_pool[200002];
    AVL::_reserve(500000);
    uint32_t n, m, id = 1;
    cin >> n >> m;
    auto add = [&](uint32_t i, uint32_t key, uint64_t val) {
        if (val && !avl_pool[i].modify_by_key(key, [val](node *p) { p->m_count += val; })) {
            avl_pool[i].insert_by_key(key, [val](node *p) { p->m_count = val; });
        }
    };
    for (uint32_t i = 1; i <= n; i++)
        avl_pool[1].insert_by_key(i, [](node *p) { cin >> p->m_count; });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, l, r;
            cin >> a >> l >> r;
            auto S = avl_pool[a].split_by_key(r + 1);
            avl_pool[++id] = avl_pool[a].split_by_key(l);
            avl_pool[a].join(S);
        } else if (op == '1') {
            uint32_t a, b;
            cin >> a >> b;
            avl_pool[a].merge(avl_pool[b]);
        } else if (op == '2') {
            uint32_t a, v;
            uint64_t cnt;
            cin >> a >> cnt >> v;
            add(a, v, cnt);
        } else if (op == '3') {
            uint32_t a, l, r;
            cin >> a >> l >> r;
            auto S2 = avl_pool[a].split_by_key(r + 1);
            auto S1 = avl_pool[a].split_by_key(l);
            cout << S1.root()->m_sum << '\n';
            avl_pool[a].join(S1), avl_pool[a].join(S2);
        } else {
            uint32_t a;
            uint64_t k;
            cin >> a >> k;
            if (avl_pool[a].root()->m_sum < k)
                cout << "-1\n";
            else {
                struct Getter {
                    using value_type = uint64_t;
                    value_type operator()() const { return 0; }
                    void tree(value_type &x, node *y) const { x += y->m_sum; }
                    void node(value_type &x, node *y) const { x += y->m_count; }
                };
                auto pos = avl_pool[a].max_right<Getter>(0, [&](auto v) { return v < k; }) + 1;
                cout << avl_pool[a].kth(pos)->m_val << '\n';
            }
        }
    }
}

int main() {
    solve_counter();
    // solve_seg();
    // solve_splay();
    // solve_avl();
}