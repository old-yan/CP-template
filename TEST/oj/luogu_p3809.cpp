#include "DS/FHQTreap.h"
#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/HashLCP.h"
#include "STR/SAM.h"
#include "STR/SuffixArray.h"
#include "STR/SuffixTree.h"

/*
[P3809 【模板】后缀排序](https://www.luogu.com.cn/problem/P3809)
*/
/**
 * 本题为后缀数组模板题，可以有多种做法
 * 可以使用哈希二分来实现
 */

void solve_SA() {
    std::string s;
    cin >> s;
    OY::SA::SuffixArray<false, false, 1000000> SA(s);
    for (uint32_t i = 0; i < s.size(); i++) cout << SA.query_sa(i) + 1 << ' ';
}

void solve_hash() {
    using mint = OY::mintu32;
    using lcp_type = OY::HASHLCP::LCP<std::string, mint, 131>;
    using table_type = lcp_type::table_type;
    using hash_type = table_type::hash_type;
    std::string s;
    cin >> s;
    hash_type::s_info.prepare_unit(s.size());
    lcp_type LCP(s);

    uint32_t n = s.size();
    std::vector<uint32_t> indexes(n);
    std::iota(indexes.begin(), indexes.end(), 0);
    std::stable_sort(indexes.begin(), indexes.end(), [&](uint32_t x, uint32_t y) {
        if (s[x] < s[y]) return true;
        if (s[x] > s[y]) return false;
        if (s[x + 1] < s[y + 1]) return true;
        if (s[x + 1] > s[y + 1]) return false;
        return LCP.compare(x, n - 1, y, n - 1) < 0;
    });
    for (auto i : indexes) cout << i + 1 << ' ';
}

template <typename Node>
struct FHQ_NodeWrap {
    using key_type = uint32_t;
    uint32_t m_key, m_val;
    void set(uint32_t key) { m_key = key; }
    const uint32_t &get() const { return m_key; }
};
struct Node {
    OY::FHQ::Multiset<FHQ_NodeWrap, 3000001> m_child;
    void set_child(uint32_t index, uint32_t child) {
        if (!m_child.modify_by_key(index, [&](auto p) { p->m_val = child; }))
            m_child.insert_by_key(index, [&](auto p) { p->m_val = child; });
    }
    uint32_t get_child(uint32_t index) const {
        auto it = m_child.lower_bound(index);
        return !it->is_null() && it->m_key == index ? it->m_val : 0;
    }
    void copy_children(const Node &rhs) {
        rhs.m_child.do_for_each([&](auto p) {
            m_child.insert_by_key(p->m_key, [&](auto q) { q->m_val = p->m_val; });
        });
    }
};
void solve_STree() {
    std::string s;
    cin >> s;
    int cnt[128]{};
    for (char c : s) cnt[c] = 1;
    cnt[0] = 0;
    std::partial_sum(cnt, cnt + 128, cnt);

    // 由于本题空间紧张，所以分情况处理一下
    // 当字符种类数很少时，使用静态结点
    // 当字符种类数很多时，使用平衡树结点
    auto solve_static = [&](auto &&S) {
        static constexpr uint32_t child_cnt = sizeof(S.m_data[0].m_child) / sizeof(S.m_data[0].m_child[0]);
        S.resize(s.size() + 1, [&](uint32_t i) { return cnt[s[i]]; });

        auto dfs = [&](auto self, uint32_t cur, uint32_t len) -> void {
            auto p = S.get_node(cur);
            if (S.is_leaf(p))
                len += s.size() - p->m_pos;
            else
                len += p->m_length;
            if (!S.is_leaf(p)) {
                for (uint32_t i = 0; i < child_cnt; i++) {
                    if (p->get_child(i)) self(self, p->get_child(i), len);
                }
            } else if (len)
                cout << s.size() - len + 1 << ' ';
        };
        dfs(dfs, 0, 0);
    };
    auto solve_dynamic = [&](auto &&S) {
        S.resize(s.size() + 1, [&](uint32_t i) { return cnt[s[i]]; });

        auto dfs = [&](auto self, uint32_t cur, uint32_t len) -> void {
            auto p = S.get_node(cur);
            if (S.is_leaf(p))
                len += s.size() - p->m_pos;
            else
                len += p->m_length;
            if (!S.is_leaf(p))
                p->m_child.do_for_each([&](auto q) {
                    self(self, q->m_val, len);
                });
            else if (len)
                cout << s.size() - len + 1 << ' ';
        };
        dfs(dfs, 0, 0);
    };
    if (cnt[127] <= 3)
        solve_static(OY::StaticSufTree_string<4>());
    else
        solve_dynamic(OY::SUFTREE::Tree<Node, std::string>());
}

int main() {
    solve_SA();
    // solve_hash();
    // solve_STree();
}