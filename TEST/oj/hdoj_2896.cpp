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
    using AC = OY::ACAM<128>;
    AC ac;
    ac.reserve(n * 200);
    std::vector<int> index(n * 200 + 1);
    for (uint32_t i = 1; i <= n; i++) {
        std::string s;
        cin >> s;
        int pos = ac.insert(s.begin(), s.end(), [](char c) { return c; });
        index[pos] = i;
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
            if (index[last_pos]) virus.insert(index[last_pos]);
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