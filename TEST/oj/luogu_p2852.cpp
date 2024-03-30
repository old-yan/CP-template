#include "DS/FHQTreap.h"
#include "DS/GlobalHashMap.h"
#include "IO/FastIO.h"
#include "MATH/OverflowUnsigned.h"
#include "STR/SAM.h"
#include "STR/StrHash.h"
#include "STR/SuffixArray.h"
#include "STR/SuffixTree.h"

/*
[P2852 [USACO06DEC] Milk Patterns G](https://www.luogu.com.cn/problem/P2852)
*/
/**
 * 本题为经典子串问题，给定子串出现次数，在至少出现这个次数的子串里找最长，可以有多种做法
 * 可以使用后缀数组解决
 * 可以用字符串哈希解决
 * 可以使用后缀树解决
 * 可以使用后缀自动机解决
 */

template <typename Node>
struct FHQ_NodeWrap {
    using key_type = uint32_t;
    uint32_t m_key, m_val;
    void set(uint32_t key) { m_key = key; }
    const uint32_t &get() const { return m_key; }
};
struct Node {
    OY::FHQ::Multiset<FHQ_NodeWrap, 60001> m_child;
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
void solve_SAM() {
    uint32_t n, k;
    cin >> n >> k;
    struct Node_cnt : Node {
        uint32_t m_cnt{};
    };
    using SAM = OY::SAM::Automaton<Node_cnt>;
    SAM sam;
    sam.reserve(n);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        sam.push_back(x);
        sam.get_node(sam.query_node_index(i))->m_cnt++;
    }
    sam.prepare();

    uint32_t ans = 0;
    sam.do_for_failing_nodes([&](uint32_t a) {
        auto p = sam.get_node(a);
        if (p->m_cnt >= k) ans = std::max(ans, p->m_length);
        if (~p->m_fail) sam.get_fail_node(a)->m_cnt += p->m_cnt;
    });
    cout << ans;
}

void solve_sa() {
    uint32_t n, k;
    cin >> n >> k;
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) cin >> a;
    OY::SA::SuffixArray<true, true, 20000> sa(arr.begin(), arr.end());
    std::vector<uint32_t> stack;
    uint32_t cursor = 0;
    uint32_t ans = 0;
    for (uint32_t l = 1, r = 1; l + k - 1 <= n; l++) {
        if (cursor < stack.size() && stack[cursor] < l) cursor++;
        while (r < l + k - 1) {
            while (cursor < stack.size() && sa.query_height(stack.back()) > sa.query_height(r)) stack.pop_back();
            stack.push_back(r++);
        }
        ans = std::max(ans, sa.query_height(stack[cursor]));
    }
    cout << ans << endl;
}

using mint = OY::mintu32;
using table_type = OY::STRHASH::StrHashPresumTable<mint, 131>;
using hash_type = table_type::hash_type;
namespace OY {
    namespace GHASH {
        template <size_type L>
        struct Hash<hash_type, L> {
            size_type operator()(const auto &x) const { return Hash<size_t, L>()(*(size_t *)(&x)); }
        };
    }
}
OY::GHASH::UnorderedMap<hash_type, uint32_t, true, 17> GS;
void solve_hash() {
    uint32_t n, k;
    cin >> n >> k;
    hash_type::s_info.prepare_unit(n), hash_type::s_info.prepare_unit_inv(n);
    std::vector<uint32_t> arr(n);
    for (auto &a : arr) cin >> a;
    table_type S(arr);
    auto check = [&](int len) {
        GS.clear();
        for (int l = 0, r = len - 1; r < arr.size(); l++, r++) {
            auto [ptr, flag] = GS.insert(S.query_hash(l, r));
            if (flag)
                ptr->m_mapped = 1;
            else
                ++(ptr->m_mapped);
            if (ptr->m_mapped == k) return true;
        }
        return false;
    };
    uint32_t low = 0, high = n - k + 1;
    while (low < high) {
        auto mid = (low + high + 1) / 2;
        if (check(mid))
            low = mid;
        else
            high = mid - 1;
    }
    cout << low << endl;
}

void solve_STree() {
    uint32_t n, k;
    cin >> n >> k;
    using STree = OY::SUFTREE::Tree<Node, std::vector<uint32_t>>;
    STree S;
    S.reserve(n + 1);
    for (uint32_t i = 0; i < n; i++) {
        uint32_t x;
        cin >> x;
        S.push_back(x);
    }
    S.push_back(-1);

    uint32_t ans = 0;
    auto dfs = [&](auto self, uint32_t cur, uint32_t len) -> uint32_t {
        auto p = S.get_node(cur);
        uint32_t cnt = 0;
        if (STree::is_leaf(p))
            len += n - p->m_pos, cnt++;
        else
            len += p->m_length;
        p->m_child.do_for_each([&](auto q) {
            cnt += self(self, q->m_val, len);
        });
        if (cnt >= k) ans = std::max(ans, len);
        return cnt;
    };
    dfs(dfs, 0, 0);
    cout << ans;
}

int main() {
    solve_SAM();
    // solve_sa();
    // solve_hash();
    // solve_STree();
}