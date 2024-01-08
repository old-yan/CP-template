#include "IO/FastIO.h"
#include "STR/ACAutomaton.h"

/*
[Keywords Search](https://acm.hdu.edu.cn/showproblem.php?pid=2222)
*/
/**
 * 本题注意可能有相同的模式串，且计多次
 */

int main() {
    uint32_t t;
    cin >> t;
    while (t--) {
        uint32_t n;
        cin >> n;
        using AC = OY::ACAM<26>;
        AC ac;
        ac.reserve(n * 50);
        std::vector<int> cnt(n * 50 + 1);
        std::vector<bool> vis(n * 50 + 1);
        for (uint32_t i = 1; i <= n; i++) {
            std::string s;
            cin >> s;
            int pos = ac.insert_lower(s);
            cnt[pos]++;
        }
        ac.prepare();

        std::string s;
        cin >> s;
        int last_pos = 0;
        uint32_t ans = 0;
        for (char c : s) {
            last_pos = ac.next(last_pos, c - 'a');
            vis[last_pos] = true;
        }

        // 在 fail 树上从叶到根倒推
        ac.do_for_failing_nodes([&](uint32_t a) {
            if (vis[a]) {
                ans += cnt[a];
                vis[ac.query_fail(a)] = true;
            }
        });

        cout << ans << endl;
    }
}