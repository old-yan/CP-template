#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

#include <set>

/*
[病毒侵袭](https://acm.hdu.edu.cn/showproblem.php?pid=2896)
*/

struct Node {
    uint32_t m_index;
};
int main() {
    uint32_t n;
    cin >> n;
    using ACAM = OY::ACAM<Node, 128>;
    ACAM ac;
    ac.reserve(n * 200);
    for (uint32_t i = 1; i <= n; i++) {
        std::string s;
        cin >> s;
        int pos = ac.insert(s.begin(), s.end(), [](char c) { return c; });
        ac.get_node(pos)->m_index = i;
    }
    ac.prepare();

    uint32_t m;
    cin >> m;
    int ans = 0;
    for (uint32_t i = 1; i <= m; i++) {
        std::string s;
        cin >> s;
        std::set<int> virus;
        int last_pos = 0;
        for (char c : s) {
            last_pos = ac.next(last_pos, c);
            if (ac.get_node(last_pos)->m_index) virus.insert(ac.get_node(last_pos)->m_index);
        }
        if (!virus.empty()) {
            ans++;
            cout << "web " << i << ":";
            for (auto a : virus) cout << ' ' << a;
            cout << endl;
        }
    }
    cout << "total: " << ans << endl;
}