#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[Keywords Search](https://acm.hdu.edu.cn/showproblem.php?pid=2222)
*/
/**
 * 本题注意可能有相同的模式串，且计多次
*/

template <typename Node>
struct NodeWrap {
    uint32_t m_cnt;
    bool m_vis;
};
int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n;
        cin >> n;
        using ACAM = OY::ACAM<NodeWrap, 26>;
        ACAM ac;
        ac.reserve(n * 50);
        for (uint32_t i = 1; i <= n; i++) {
            std::string s;
            cin >> s;
            int pos = ac.insert_lower(s);
            ac.get_node(pos)->m_cnt++;
        }
        ac.prepare();

        std::string s;
        cin >> s;
        int last_pos = 0;
        uint32_t ans = 0;
        for (char c : s) {
            last_pos = ac.next(last_pos, c - 'a');
            ac.get_node(last_pos)->m_vis = true;
        }

        // 在 fail 树上从叶到根倒推
        ac.do_for_failing_nodes([&](uint32_t a) {
            if (ac.get_node(a)->m_vis) {
                ans += ac.get_node(a)->m_cnt;
                ac.get_fail_node(a)->m_vis = true;
            }
        });

        cout << ans << endl;
    }
}