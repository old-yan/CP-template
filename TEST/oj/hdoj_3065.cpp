#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[病毒侵袭持续中(https://acm.hdu.edu.cn/showproblem.php?pid=3065)
*/

template <typename Node>
struct NodeWrap {
    uint32_t m_index, m_cnt;
};
int main() {
    uint32_t n;
    while (cin >> n) {
        using ACAM = OY::ACAM<NodeWrap, 27>;
        ACAM ac;

        std::vector<std::string> words(n);
        for (auto &a : words) cin >> a;

        ac.reserve(n * 50);
        for (uint32_t i = 0; i < n; i++) {
            int pos = ac.insert(words[i].begin(), words[i].end(), [](char c) { return c - 'A'; });
            ac.get_node(pos)->m_index = i + 1;
        }
        ac.prepare();

        std::string s;
        cin >> s;
        int last_pos = 0;
        for (char c : s) {
            last_pos = ac.next(last_pos, c >= 'A' && c <= 'Z' ? c - 'A' : 26);
            ac.get_node(last_pos)->m_cnt++;
        }

        // 在 fail 树上从叶到根倒推
        std::vector<uint32_t> ans_cnt(n);
        ac.do_for_failing_nodes([&](uint32_t a) {
            auto p = ac.get_node(a);
            if (p->m_cnt) {
                if (p->m_index)
                    ans_cnt[p->m_index - 1] = p->m_cnt;
                ac.get_fail_node(a)->m_cnt += p->m_cnt;
            }
        });

        for (uint32_t i = 0; i < n; i++)
            if (ans_cnt[i])
                cout << words[i] << ": " << ans_cnt[i] << endl;
    }
}