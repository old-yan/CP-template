#include "IO/FastIO.h"
#include "STR/BiSuffixTree.h"
#include "STR/SAM.h"
#include "STR/SuffixArray.h"
#include "STR/SuffixTree.h"

/*
[P2408 不同子串个数](https://www.luogu.com.cn/problem/P2408)
*/
/**
 * 本题为经典子串问题，求本质不同的子串数量，可以有多种做法
 * 可以套一个后缀自动机轻易解决
 * 也可以使用后缀数组解决
 * 也可以使用后缀树解决
 */

void solve_sa() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    OY::SA::SuffixArray<true, true, 100000> SA(s);

    uint64_t ans = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint32_t can = n - SA.query_sa(i);
        uint32_t h = SA.query_height(i);
        ans += can - h;
    }
    cout << ans << endl;
}

void solve_SAM() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    using SAM = OY::StaticSAM_string<26>;
    SAM sam(s.size(), [&](uint32_t i) { return s[i] - 'a'; });
    std::vector<int> cnt(sam.m_data.size());
    sam.prepare();

    sam.do_for_failing_nodes([&](uint32_t a) {
        auto p = sam.get_node(a);
        for (uint32_t i = 0; i < 26; i++)
            if (p->get_child(i)) cnt[a] += cnt[p->get_child(i)] + 1;
    });

    cout << cnt[0] << endl;
}

void solve_STree() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    using STree = OY::StaticSufTree_string<26>;
    // 在本问题中，不需要求出后缀树，只需要隐式后缀树
    STree st(s.size(), [&](uint32_t i) { return s[i] - 'a'; });

    uint64_t ans = 0;
    for (auto &node : st.m_data) {
        if (STree::is_leaf(&node))
            ans += s.size() - node.m_pos;
        else
            ans += node.m_length;
    }
    cout << ans << endl;
}

void solve_BiSTree() {
    uint32_t n;
    std::string s;
    cin >> n >> s;
    using STree = OY::StaticBiSufTree_string<uint8_t, 26, 100000>;
    // 在本问题中，不需要求出后缀树，只需要隐式后缀树
    STree st;
    st.reserve(s.size());
    for (char c : s) st.push_back(c - 'a');
    cout << st.unique_count() << endl;
}

int main() {
    solve_sa();
    // solve_SAM();
    // solve_STree();
    // solve_BiSTree();
}