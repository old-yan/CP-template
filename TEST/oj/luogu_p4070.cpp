#include "DS/FHQTreap.h"
#include "DS/IntegerSet.h"
#include "IO/FastIO.h"
#include "STR/SALCP.h"
#include "STR/SAM.h"
#include "STR/SuffixTree.h"
#include "STR/BiSuffixTree.h"

/*
[P4070 [SDOI2016] 生成魔咒](https://www.luogu.com.cn/problem/P4070)
*/
/**
 * 显然，本题为模板题，问的就是 SAM 中的结点数
 * 也可以有多种做法，比如用后缀数组解决
 * 用后缀树来解决是最简单的
 */

template <typename Node>
struct FHQ_NodeWrap {
    using key_type = uint32_t;
    uint32_t m_key, m_val;
    void set(uint32_t key) { m_key = key; }
    const uint32_t &get() const { return m_key; }
};
struct MapNode {
    OY::FHQ::Multiset<FHQ_NodeWrap, 600001> m_child;
    void set_child(uint32_t index, uint32_t child) {
        if (!m_child.modify_by_key(index, [&](auto p) { p->m_val = child; }))
            m_child.insert_by_key(index, [&](auto p) { p->m_val = child; });
    }
    uint32_t get_child(uint32_t index) const {
        auto it = m_child.lower_bound(index);
        return it->m_key == index ? it->m_val : 0;
    }
    void copy_children(const MapNode &rhs) {
        rhs.m_child.do_for_each([&](auto p) {
            m_child.insert_by_key(p->m_key, [&](auto q) { q->m_val = p->m_val; });
        });
    }
};
void solve_SAM() {
    using SAM = OY::SAM::Automaton<MapNode>;
    SAM sam;
    uint32_t n;
    cin >> n;
    sam.reserve(n);

    uint64_t ans = 0;
    /**
     * 一个知识点：如何动态收集本质不同子串数，如何单独计算新插入的字符的贡献
     */
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        sam.push_back(x);
        uint32_t a = sam.query_node_index(i), f = sam.query_fail(a);
        ans += sam.get_node(a)->m_length;
        if (~f) ans -= sam.get_node(f)->m_length;
        cout << ans << endl;
    }
}

void solve_sa() {
    uint32_t n;
    cin >> n;
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) cin >> a;
    // 将原序列倒置
    OY::SALCP::LCP<100000> LCP(arr.rbegin(), arr.rend());
    auto &SA = LCP.m_table;
    auto &st = LCP.m_inner_table;
    OY::INTSET::Table<17> S;

    // 每次用总子串数，减去重复数
    uint64_t collision = 0;
    for (uint32_t i = n - 1; ~i; i--) {
        uint32_t rnk = SA.query_rank(i);
        S.set(rnk);
        uint32_t pre = S.smaller_bound(rnk), nxt = S.upper_bound(rnk);
        if (~pre)
            if (~nxt) {
                collision += st.query(pre + 1, rnk);
                collision += st.query(rnk + 1, nxt);
                collision -= st.query(pre + 1, nxt);
            } else
                collision += st.query(pre + 1, rnk);
        else if (~nxt)
            collision += st.query(rnk + 1, nxt);
        cout << uint64_t(n - i) * (n - i + 1) / 2 - collision << endl;
    }
}

void solve_STree() {
    using STree = OY::SUFTREE::Tree<MapNode, std::vector<uint32_t>>;
    STree st;
    uint32_t n;
    cin >> n;
    st.reserve(n);

    uint64_t ans = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        st.push_back(x);
        ans += st.leaf_count();
        cout << ans << endl;
    }
}

void solve_BiSTree() {
    using BiSTree = OY::BISUFTREE::Tree<uint32_t, MapNode, 100000>;
    BiSTree st;
    uint32_t n;
    cin >> n;
    st.reserve(n);

    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        st.push_back(x);
        cout << st.unique_count() << endl;
    }
}

int main() {
    solve_SAM();
    // solve_sa();
    // solve_STree();
    // solve_BiSTree();
}