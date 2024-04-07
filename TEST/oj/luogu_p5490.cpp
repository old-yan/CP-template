#include "DS/Discretizer.h"
#include "DS/SegTree.h"
#include "DS/ZkwTree.h"
#include "IO/FastIO.h"

/*
[P5490 【模板】扫描线](https://www.luogu.com.cn/problem/P5490)
*/
/**
 * 扫描线问题，可以用线段树解决
 */

struct event {
    uint32_t t, low, high;
    bool is_add;
    bool operator<(const event &rhs) const { return t < rhs.t; }
};
template <typename Tp>
struct SegNode {
    using value_type = Tp;
    using modify_type = bool;
    using node_type = SegNode<Tp>;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    template <typename real_node_type>
    static void map(const modify_type &modify, real_node_type *x, Tp len) {
        if (modify)
            x->m_val = len;
        else if (x->m_modify == 1)
            x->m_val = x->lchild()->m_val + x->rchild()->m_val;
    }
    static void com(const modify_type &modify, node_type *x) {
        if (modify)
            x->m_modify++;
        else
            x->m_modify--;
    }
    value_type m_val;
    uint32_t m_modify;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_val = val; }
    void pushup(node_type *lchild, node_type *rchild, Tp len) { m_val = m_modify ? len : lchild->m_val + rchild->m_val; }
};
void solve_seg() {
    uint32_t n;
    cin >> n;
    std::vector<event> evs;
    evs.reserve(n * 2);
    uint32_t maxy = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        evs.push_back({x1, y1, y2 - 1, true});
        evs.push_back({x2, y1, y2 - 1, false});
        maxy = std::max(maxy, y2);
    }
    std::sort(evs.begin(), evs.end());

    OY::Seg::Tree<SegNode<uint32_t>, OY::Seg::Ignore, false, uint32_t, 1 << 23> S(maxy + 1);
    int64_t h = 0, last = 0, ans = 0;
    for (auto &[t, low, high, is_add] : evs) {
        ans += h * (t - last);
        last = t;
        S.add(low, high, is_add);
        h = S.query_all();
    }
    cout << ans << endl;
}

template <typename Tp>
struct ZkwNode {
    using value_type = Tp;
    using modify_type = bool;
    using node_type = ZkwNode<Tp>;
    static value_type op(const value_type &x, const value_type &y) { return x + y; }
    static void map(const modify_type &modify, node_type *x, uint32_t len) {
        if (modify)
            x->m_modify++, x->m_val = x->m_area;
        else if (!--x->m_modify)
            x->m_val = x->m_sum;
    }
    static void com(const modify_type &modify, node_type *x) {}
    Tp m_val, m_sum, m_area;
    uint32_t m_modify;
    const value_type &get() const { return m_val; }
    void set(const value_type &val) { m_area = val; }
    void init_pushup(node_type *lchild, node_type *rchild) {
        m_area = lchild->m_area + rchild->m_area;
    }
    void pushup(node_type *lchild, node_type *rchild) {
        m_sum = lchild->m_val + rchild->m_val;
        m_val = m_modify ? m_area : m_sum;
    }
};
void solve_zkw() {
    uint32_t n;
    cin >> n;
    std::vector<event> evs;
    evs.reserve(n * 2);
    uint32_t maxy = 0;
    OY::Discretizer<uint32_t> D;
    D.reserve(n * 2);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        evs.push_back({x1, y1, y2 - 1, true});
        evs.push_back({x2, y1, y2 - 1, false});
        maxy = std::max(maxy, y2 - 1);
        D << y1 << y2;
    }
    std::sort(evs.begin(), evs.end());
    D.prepare();

    OY::Zkw::Tree<ZkwNode<uint32_t>, 1 << 21> S(D.size() - 1, [&](uint32_t i) {
        return D[i + 1] - D[i];
    });
    int64_t h = 0, last = 0, ans = 0;
    for (auto &[t, low, high, is_add] : evs) {
        ans += h * (t - last);
        last = t;
        S.add(D.rank(low), D.rank(high + 1) - 1, is_add);
        h = S.query_all();
    }
    cout << ans << endl;
}

int main() {
    solve_zkw();
    // solve_seg();
}