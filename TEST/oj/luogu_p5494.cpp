#include "DS/FHQTreap.h"
#include "DS/SegTree.h"
#include "IO/FastIO.h"

/*
[P5494 【模板】线段树分裂](https://www.luogu.com.cn/problem/P5494)
*/
/**
 * 本题为线段树分裂/合并模板题
 * 涉及到分裂合并，只能使用动态开点版本，不能使用 Zkw
 */
using Seg = OY::Seg::Tree<OY::Seg::BaseNode<uint64_t>, OY::Seg::Ignore, false, uint32_t, 1 << 22>;
Seg seg_pool[200002];
void solve_seg() {
    uint32_t n, m, cnt = 2;
    cin >> n >> m;
    seg_pool[1].resize(n, [&](auto...) {
        uint64_t x;
        cin >> x;
        return x;
    });
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
using FHQ = OY::FHQ::Multiset<NodeWrap>;
using node = FHQ::node;
FHQ fhq_pool[200002];
void solve_fhq() {
    uint32_t n, m, id = 1;
    cin >> n >> m;
    auto add = [](uint32_t i, uint32_t key, uint64_t val) {
        if (val && !fhq_pool[i].modify_by_key(key, [val](node *p) { p->m_count += val; })) fhq_pool[i].insert_by_key(key, [val](node *p) { p->m_count = val; });
    };
    for (uint32_t i = 1; i <= n; i++)
        fhq_pool[1].insert_by_key(i, [](node *p) { cin >> p->m_count; });
    for (uint32_t i = 0; i < m; i++) {
        char op;
        cin >> op;
        if (op == '0') {
            uint32_t a, l, r;
            cin >> a >> l >> r;
            auto S = fhq_pool[a].split_by_key(r + 1);
            fhq_pool[++id] = fhq_pool[a].split_by_key(l);
            fhq_pool[a].join(S);
        } else if (op == '1') {
            uint32_t a, b;
            cin >> a >> b;
            fhq_pool[a].merge(fhq_pool[b]);
        } else if (op == '2') {
            uint32_t a, v;
            uint64_t cnt;
            cin >> a >> cnt >> v;
            add(a, v, cnt);
        } else if (op == '3') {
            uint32_t a, l, r;
            cin >> a >> l >> r;
            auto S2 = fhq_pool[a].split_by_key(r + 1);
            auto S1 = fhq_pool[a].split_by_key(l);
            cout << S1.root()->m_sum << '\n';
            fhq_pool[a].join(S1), fhq_pool[a].join(S2);
        } else {
            uint32_t a;
            uint64_t k;
            cin >> a >> k;
            if (fhq_pool[a].root()->m_sum < k)
                cout << "-1\n";
            else {
                struct SumJudger {
                    uint64_t m_sum, m_ceil;
                    SumJudger(uint64_t ceil) : m_sum(), m_ceil(ceil) {}
                    bool try_lchild(node *x) {
                        uint64_t tmp = x->lchild()->m_sum + m_sum;
                        if (tmp > m_ceil) return false;
                        return m_sum = tmp, true;
                    }
                    // 求 max_right 其实不需要实现 try_rchild，求 min_left 才会用到
                    bool try_rchild(node *x) {
                        uint64_t tmp = m_sum + x->rchild()->m_sum;
                        if (tmp > m_ceil) return false;
                        return m_sum = tmp, true;
                    }
                    bool try_mid(node *x) {
                        uint64_t tmp = m_sum + x->m_count;
                        if (tmp > m_ceil) return false;
                        return m_sum = tmp, true;
                    }
                };
                auto pos = fhq_pool[a].max_right(0, SumJudger(k - 1)) + 1;
                cout << fhq_pool[a].kth(pos)->m_val << '\n';
            }
        }
    }
}

int main() {
    solve_seg();
    // solve_fhq();
}